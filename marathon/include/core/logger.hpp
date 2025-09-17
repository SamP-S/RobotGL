#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace marathon {

namespace core {

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
#define MT_CORE_TRACE(...)      ::marathon::core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MT_CORE_DEBUG(...)      ::marathon::core::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define MT_CORE_INFO(...)       ::marathon::core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MT_CORE_WARN(...)       ::marathon::core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MT_CORE_ERROR(...)      ::marathon::core::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MT_CORE_CRITICAL(...)   ::marathon::core::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define MT_CORE_SET_LEVEL(lvl)  ::marathon::core::Log::GetCoreLogger()->set_level(static_cast<spdlog::level::level_enum>(lvl))
#define MT_CORE_GET_LEVEL()     static_cast<::marathon::core::Log::Level>(::marathon::core::Log::GetCoreLogger()->level())

// Engine log macros
#define MT_ENGINE_TRACE(...)        ::marathon::core::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define MT_ENGINE_DEBUG(...)        ::marathon::core::Log::GetEngineLogger()->debug(__VA_ARGS__)
#define MT_ENGINE_INFO(...)         ::marathon::core::Log::GetEngineLogger()->info(__VA_ARGS__)
#define MT_ENGINE_WARN(...)         ::marathon::core::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define MT_ENGINE_ERROR(...)        ::marathon::core::Log::GetEngineLogger()->error(__VA_ARGS__)
#define MT_ENGINE_CRITICAL(...)     ::marathon::core::Log::GetEngineLogger()->critical(__VA_ARGS__)
#define MT_ENGINE_SET_LEVEL(lvl)    ::marathon::core::Log::GetEngineLogger()->set_level(static_cast<spdlog::level::level_enum>(lvl))
#define MT_ENGINE_GET_LEVEL()       static_cast<::marathon::core::Log::Level>(::marathon::core::Log::GetEngineLogger()->level())

// User log macros
#define MT_TRACE(...)       ::marathon::core::Log::GetUserLogger()->trace(__VA_ARGS__)
#define MT_DEBUG(...)       ::marathon::core::Log::GetUserLogger()->debug(__VA_ARGS__)
#define MT_INFO(...)        ::marathon::core::Log::GetUserLogger()->info(__VA_ARGS__)
#define MT_WARN(...)        ::marathon::core::Log::GetUserLogger()->warn(__VA_ARGS__)
#define MT_ERROR(...)       ::marathon::core::Log::GetUserLogger()->error(__VA_ARGS__)
#define MT_CRITICAL(...)    ::marathon::core::Log::GetUserLogger()->critical(__VA_ARGS__)
#define MT_SET_LEVEL(lvl)   ::marathon::core::Log::GetUserLogger()->set_level(static_cast<spdlog::level::level_enum>(lvl))
#define MT_GET_LEVEL()      static_cast<::marathon::core::Log::Level>(::marathon::core::Log::GetUserLogger()->level())

} // core

} // marathon