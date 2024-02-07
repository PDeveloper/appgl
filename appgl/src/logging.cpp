#include "appgl/logging.hpp"

#include <unordered_map>

namespace appgl {

namespace logging {

std::unordered_map<std::string, logger_ptr> global_loggers;

void init(size_t queue_size, size_t thread_count, size_t flush_interval_ms) {
    spdlog::init_thread_pool(queue_size, thread_count);
    spdlog::flush_every(std::chrono::milliseconds(flush_interval_ms));
}

logger_ptr create(const std::string& name, spdlog::level::level_enum level, const std::string& pattern)
{
    try {
        auto logger = global_loggers.emplace(name, spdlog::create_async_nb<spdlog::sinks::stdout_color_sink_mt>(name)).first->second;
        logger->set_level(level);
        logger->set_pattern(pattern);
        return logger;
    } catch (const spdlog::spdlog_ex &ex) {
        spdlog::error("Log initialization failed: {}", ex.what());
    }
    return nullptr;
}

logger_ptr get_or_create(const std::string& name, spdlog::level::level_enum level) {
    if (global_loggers.find(name) == global_loggers.end()) {
        return create(name, level);
    }
    return global_loggers[name];
}

logger_ptr get(const std::string& name) {
    return global_loggers[name];
}

} // namespace logging

logging::logger_ptr logger(const std::string& name) {
    return logging::get(name);
}

} // namespace appgl
