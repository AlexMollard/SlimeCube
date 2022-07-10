#pragma once

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

class Log
{
public:
 static void Init();

 static void Trace(std::string message);

 static void Info(std::string message);

 static void Warn(std::string message);

 static void Error(std::string message);

private:
 inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return coreLogger; }

 inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return clientLogger; }

 static std::shared_ptr<spdlog::logger> coreLogger;
 static std::shared_ptr<spdlog::logger> clientLogger;
};
