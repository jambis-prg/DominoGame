#pragma once

enum class LogTypes
{
    Info,
    Debug,
    Warning,
    Error,
    Fatal
};

void Log(LogTypes type, const char *message, ...);

#ifdef _DEBUG
    #define LOG_INFO(...) Log(LogTypes::Info, __VA_ARGS__)
    #define LOG_DEBUG(...) Log(LogTypes::Debug, __VA_ARGS__)
    #define LOG_WARN(...) Log(LogTypes::Warn, __VA_ARGS__)
    #define LOG_ERROR(...) Log(LogTypes::Error, __VA_ARGS__)
    #define LOG_FATAL(...) Log(LogTypes::Fatal, __VA_ARGS__)
#else
    #define LOG_INFO(message, ...)
    #define LOG_DEBUG(message, ...)
    #define LOG_WARN(message, ...)
    #define LOG_ERROR(message, ...)
    #define LOG_FATAL(message, ...)
#endif