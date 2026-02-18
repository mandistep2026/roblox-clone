#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace roblox_clone::core {

class Logger {
public:
    static void init();
    static void setLevel(spdlog::level::level_enum level);
    
    static std::shared_ptr<spdlog::logger>& getLogger() { return s_logger; }

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

}

#define RC_TRACE(...)    ::roblox_clone::core::Logger::getLogger()->trace(__VA_ARGS__)
#define RC_DEBUG(...)    ::roblox_clone::core::Logger::getLogger()->debug(__VA_ARGS__)
#define RC_INFO(...)     ::roblox_clone::core::Logger::getLogger()->info(__VA_ARGS__)
#define RC_WARN(...)     ::roblox_clone::core::Logger::getLogger()->warn(__VA_ARGS__)
#define RC_ERROR(...)    ::roblox_clone::core::Logger::getLogger()->error(__VA_ARGS__)
#define RC_CRITICAL(...) ::roblox_clone::core::Logger::getLogger()->critical(__VA_ARGS__)
