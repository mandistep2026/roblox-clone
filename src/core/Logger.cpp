#include "Logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace roblox_clone::core {

std::shared_ptr<spdlog::logger> Logger::s_logger;

void Logger::init() {
    s_logger = spdlog::stdout_color_mt("ROBLOX_CLONE");
    s_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
    s_logger->set_level(spdlog::level::debug);
}

void Logger::setLevel(spdlog::level::level_enum level) {
    s_logger->set_level(level);
}

}
