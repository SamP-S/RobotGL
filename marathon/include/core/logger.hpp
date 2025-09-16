#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace marathon {

/// NOTE: Inspired by the Log class from The Cherno's Hazel engine
/// CONSIDER: wrapping all spdlog functionality in a logger class
class Log {
private:
    static std::shared_ptr<spdlog::logger> s_coreLogger;
    static std::shared_ptr<spdlog::logger> s_engineLogger;
    static std::shared_ptr<spdlog::logger> s_userLogger;

    // init all loggers
    static void Init();

public:
    enum Level {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL,
        DISABLED,
        NUM_LEVELS
    };

    // get relevant loggers for application level
    static std::shared_ptr<spdlog::logger> GetCoreLogger();
    static std::shared_ptr<spdlog::logger> GetEngineLogger();
    static std::shared_ptr<spdlog::logger> GetUserLogger();
};

// Core log macros
#define MT_CORE_TRACE(...)      ::marathon::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MT_CORE_DEBUG(...)      ::marathon::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define MT_CORE_INFO(...)       ::marathon::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MT_CORE_WARN(...)       ::marathon::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MT_CORE_ERROR(...)      ::marathon::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MT_CORE_CRITICAL(...)   ::marathon::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define MT_CORE_SET_LEVEL(lvl)  ::marathon::Log::GetCoreLogger()->set_level(static_cast<spdlog::level::level_enum>(lvl))
#define MT_CORE_GET_LEVEL()     static_cast<::marathon::Log::Level>(::marathon::Log::GetCoreLogger()->level())

// Engine log macros
#define MT_ENGINE_TRACE(...)        ::marathon::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define MT_ENGINE_DEBUG(...)        ::marathon::Log::GetEngineLogger()->debug(__VA_ARGS__)
#define MT_ENGINE_INFO(...)         ::marathon::Log::GetEngineLogger()->info(__VA_ARGS__)
#define MT_ENGINE_WARN(...)         ::marathon::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define MT_ENGINE_ERROR(...)        ::marathon::Log::GetEngineLogger()->error(__VA_ARGS__)
#define MT_ENGINE_CRITICAL(...)     ::marathon::Log::GetEngineLogger()->critical(__VA_ARGS__)
#define MT_ENGINE_SET_LEVEL(lvl)    ::marathon::Log::GetEngineLogger()->set_level(static_cast<spdlog::level::level_enum>(lvl))
#define MT_ENGINE_GET_LEVEL()       static_cast<::marathon::Log::Level>(::marathon::Log::GetEngineLogger()->level())

// User log macros
#define MT_TRACE(...)       ::marathon::Log::GetUserLogger()->trace(__VA_ARGS__)
#define MT_DEBUG(...)       ::marathon::Log::GetUserLogger()->debug(__VA_ARGS__)
#define MT_INFO(...)        ::marathon::Log::GetUserLogger()->info(__VA_ARGS__)
#define MT_WARN(...)        ::marathon::Log::GetUserLogger()->warn(__VA_ARGS__)
#define MT_ERROR(...)       ::marathon::Log::GetUserLogger()->error(__VA_ARGS__)
#define MT_CRITICAL(...)    ::marathon::Log::GetUserLogger()->critical(__VA_ARGS__)
#define MT_SET_LEVEL(lvl)   ::marathon::Log::GetUserLogger()->set_level(static_cast<spdlog::level::level_enum>(lvl))
#define MT_GET_LEVEL()      static_cast<::marathon::Log::Level>(::marathon::Log::GetUserLogger()->level())

} // marathon