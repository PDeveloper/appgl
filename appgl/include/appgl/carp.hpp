#pragma once

#include <concepts>
#include <memory>
#include <optional>
#include <tuple>
#include <span>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace carp {

template<typename T>
concept Parseable = requires(T a, std::span<std::string_view> sv, size_t nvalues) {
	{ a.parse(sv, nvalues) } -> std::same_as<size_t>;
};

template<typename T, typename... Args>
concept EmplaceBackable = requires(T a, Args&&... args) {
	{ a.emplace_back(std::forward<Args>(args)...) };
};

template<typename T>
concept HasValueType = requires {
	typename T::value_type;
};

template<typename From, typename To>
concept ConvertibleTo = std::is_convertible_v<From, To>;

template<typename Func>
concept NoArgCallable = std::is_invocable_v<Func>;

template<typename T>
T convert_to(const std::string& str) {
	std::istringstream ss(str);
	T value;
	ss >> value;
	return value;
}

template<typename T>
T consume(std::span<T>& span) {
	T v = span.front();
	span = span.subspan(1);
	return v;
}

struct _arg_parser {
	std::string help;
	virtual size_t parse(std::span<std::string_view> args, size_t nvalues) { return 0; };
};

template<typename T, ConvertibleTo<T> U>
struct _flag : public _arg_parser {
	T& value;
	T set_value;
	_flag(T& value, U set_value = true) :
		value(value),
		set_value(set_value)
	{
	}
	size_t parse(std::span<std::string_view> args, size_t nvalues) override {
		value = set_value;
		return 0;
	}
};

template<NoArgCallable TCallback>
struct _flag_callback : public _arg_parser {
	TCallback callback;
	_flag_callback(TCallback&& callback) :
		callback(callback)
	{
	}
	size_t parse(std::span<std::string_view> args, size_t nvalues) override {
		callback();
		return 0;
	}
};

template<typename T>
struct _arg : public _arg_parser {
	T& value;
	_arg(T& value) :
		value(value)
	{
	}
	size_t parse(std::span<std::string_view> args, size_t nvalues) override {
		if (args.size() <= 0) return 0;
		std::string str_v(args.front());
		value = convert_to<T>(str_v);
		return 1;
	}
};

template<typename T, EmplaceBackable<T> TContainer>
struct _arglist : public _arg_parser {
	TContainer& container;
	_arglist(TContainer& container) :
		container(container)
	{
	}
	size_t parse(std::span<std::string_view> args, size_t nvalues) override {
		for (auto& arg : args.subspan(0, nvalues)) {
			std::string str_v(arg);
			auto value = convert_to<T>(str_v);
			container.emplace_back(value);
		}
		return nvalues;
	}
};

template<NoArgCallable TCallback>
struct _mode;

class carp_args {
public:
	void add(const std::pair<std::string, std::string>& names, std::unique_ptr<_arg_parser>&& parser, std::string help = "", std::pair<std::string, std::string> prefixes = { "--", "-" }) {
		parsers.push_back(std::move(parser));
		auto& p = parsers.back();
		p->help = help;
		arguments.emplace(prefixes.first + names.first, p.get());
		if (names.second != "") arguments.emplace(prefixes.second + names.second, p.get());
	}

	template<NoArgCallable TCallback>
	void mode(std::string name, carp_args args, TCallback&& callback, std::string help = "") {
		add({ name, "" }, std::make_unique<_mode<TCallback>>(args, std::move(callback)), help, { "","" });
	}

	template<NoArgCallable TCallback>
	void flag(const std::pair<std::string, std::string>& names, TCallback&& callback, std::string help = "") {
		add(names, std::make_unique<_flag_callback<TCallback>>(std::move(callback)), help);
	}

	template<typename T, ConvertibleTo<T> U = T>
	void flag(const std::pair<std::string, std::string>& names, T& value, U set_value = true, std::string help = "") {
		add(names, std::make_unique<_flag<T, U>>(value, set_value), help);
	}

	template<typename T>
	void arg(const std::pair<std::string, std::string>& names, T& value, std::string help = "") {
		add(names, std::make_unique<_arg<T>>(value), help);
	}

	template<HasValueType TContainer, typename T = typename TContainer::value_type>
	void args(const std::pair<std::string, std::string>& names, TContainer& container, std::string help = "") {
		add(names, std::make_unique<_arglist<T, TContainer>>(container), help);
	}

	void parse(std::span<std::string_view>& args) {
		while (!args.empty()) {
			std::string name(consume(args));

			if (!arguments.contains(name)) continue;

			size_t index;
			for (index = 0; index < args.size(); ++index) {
				std::string value(args[index]);
				if (arguments.contains(value)) break;
			}

			auto parser = arguments.at(name);
			size_t num_parsed = parser->parse(args, index);
			args = args.subspan(num_parsed);
		}
	}

protected:
	std::vector<std::unique_ptr<_arg_parser>> parsers;
	std::unordered_map<std::string, _arg_parser*> arguments;
};

template<NoArgCallable TCallback>
struct _mode : public _arg_parser {
	carp_args args;
	TCallback callback;
	_mode(carp_args args, TCallback&& callback) :
		args(args),
		callback(callback)
	{
	}
	size_t parse(std::span<std::string_view> args, size_t nvalues) override {
		callback();
		return args.size();
	}
};

class carp : public carp_args {
public:
	std::string help_format{ "   %s, %s - %s\n" };
	std::string version;

	void parse(int argc, char** argv) {
		flag({ "help", "H" }, [&]() {
			printf("Available commands:\n");
			for (auto& p : parsers) {
				std::string sf;
				std::string lf;
				for (auto& a : arguments) {
					if (a.second == p.get()) {
						if (a.first.starts_with("--")) lf = a.first;
						else if (a.first.starts_with("-")) sf = a.first;
					}
				}
				printf(help_format.c_str(), sf.c_str(), lf.c_str(), p->help.c_str());
			}
			}, "Print this help menu");
		flag({ "version", "V" }, [&]() {
			printf("%s\n", version.c_str());
			}, "Print the application version");

		std::span<char*> cargs{ argv, static_cast<size_t>(argc) };
		std::vector<std::string_view> argument_list;
		for (const auto carg : cargs) {
			argument_list.emplace_back(carg);
		}
		std::span<std::string_view> args(argument_list);
		carp_args::parse(args);
	}
};

} // namespace carp
