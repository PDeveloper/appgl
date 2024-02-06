#pragma once

#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

namespace appgl {

namespace logging {

using logger_ptr = std::shared_ptr<spdlog::logger>;

void init(size_t queue_size = 1024, size_t thread_count = 1, size_t flush_interval_ms = 1000);
logger_ptr create(const std::string& name, spdlog::level::level_enum level = spdlog::level::trace, const std::string& pattern = "%^| %b %d %H:%M:%S.%e [%n] [%L] %v%$");
logger_ptr get_or_create(const std::string& name, spdlog::level::level_enum level = spdlog::level::trace);
logger_ptr get(const std::string& name);

} // namespace logging

logging::logger_ptr logger(const std::string& name);

} // namespace appgl
