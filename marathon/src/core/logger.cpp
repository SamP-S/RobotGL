#include "core/logger.hpp"

namespace marathon {

// define static loggers
std::shared_ptr<spdlog::logger> Log::s_coreLogger;
std::shared_ptr<spdlog::logger> Log::s_engineLogger;
std::shared_ptr<spdlog::logger> Log::s_userLogger;

void Log::Init() {
    std::vector<spdlog::sink_ptr> logSinks;

    /// TODO: each logger should sink to seperate files
    /// add thread logging when needed
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("marathon.log", true));

    logSinks[0]->set_pattern("%^[%T] [%n %l]: %v %$");
    logSinks[1]->set_pattern("%^[%Y-%m-%d %H:%M:%S] [%n %l]: %v %$");

    s_coreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_coreLogger);
    s_coreLogger->set_level(spdlog::level::trace);
    s_coreLogger->flush_on(spdlog::level::trace);

    s_engineLogger = std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_engineLogger);
    s_engineLogger->set_level(spdlog::level::trace);
    s_engineLogger->flush_on(spdlog::level::trace);

    s_userLogger = std::make_shared<spdlog::logger>("USER", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_userLogger);
    s_userLogger->set_level(spdlog::level::trace);
    s_userLogger->flush_on(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Log::GetCoreLogger() {
    if (!s_coreLogger) {
        Init();
    }
    return s_coreLogger;
}
std::shared_ptr<spdlog::logger> Log::GetEngineLogger() {
    if (!s_engineLogger) {
        Init();
    }
    return s_engineLogger;
}
std::shared_ptr<spdlog::logger> Log::GetUserLogger() {
    if (!s_userLogger) {
        Init();
    }
    return s_userLogger;
}

} // marathon