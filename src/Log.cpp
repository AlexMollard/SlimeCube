#include "Log.h"

std::shared_ptr<spdlog::logger> Log::coreLogger;
std::shared_ptr<spdlog::logger> Log::clientLogger;

void Log::Init()
{
    spdlog::set_pattern("[%T] %n: %v%$");

    coreLogger = spdlog::stdout_color_mt("SLIMECUBE");
    coreLogger->set_level(spdlog::level::trace);

    clientLogger = spdlog::stdout_color_mt("APP");
    clientLogger->set_level(spdlog::level::trace);
}

void Log::Trace(std::string message) { GetCoreLogger()->trace(message); }

void Log::Info(std::string message) { GetCoreLogger()->info(message); }

void Log::Warn(std::string message) { GetCoreLogger()->warn(message); }

void Log::Error(std::string message) { GetCoreLogger()->error(message); }
