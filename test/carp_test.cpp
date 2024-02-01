#include <cstdlib>

#include "carp.hpp"

void enable_logging() {
	printf("enabling logging facilities!\n");
}

int main(int argc, char** argv)
{
	std::string output_file = "";
	bool make_static = false;
	size_t number = 0;
	std::vector<std::string> strings;
	std::vector<double> numbers;

	carp parser;
	parser.flag({ "static", "s" }, make_static, "Run a static process");
	parser.flag({ "large", "l" }, number, 64, "Set number to a large value");
	parser.arg({ "out", "o" }, output_file, "Output of all conversions");
	parser.arg({ "number", "n" }, number, "Set the number");
	parser.args({ "string", "sv" }, strings, "Append a string value to properties");
	parser.args({ "numbers", "nv" }, numbers, "Append a number to all the numbers");
	parser.flag({ "logging", "log" }, [&]() {
		printf("asdfasdfasdf\n");
		number = 999;
		}, "Enable logging v1");
	parser.flag({ "logging2", "log2" }, enable_logging, "Enable logging v2");

	parser.parse(argc, argv);

	printf("%s\n", output_file.c_str());
	printf("%s\n", make_static ? "true" : "false");
	printf("%zd\n", number);
	for (auto& string : strings) printf("%s, ", string.c_str());
	printf("\n");
	for (auto& number : numbers) printf("%s, ", std::to_string(number).c_str());
	printf("\n");

	return EXIT_SUCCESS;
}
