#pragma once

#include <string_view>

// ---------------------------------------------------------------------------
// Logger — static, no instance. Origin (file/line/func) captured via macros
// at the call site so callers never pass it manually.
// ---------------------------------------------------------------------------

enum class LogLevel { Trace, Debug, Info, Warning, Error, Fatal };

class Logger {
public:
    static void SetLevel(LogLevel level);
    static void Log(LogLevel level, const char* file, int line, const char* func, std::string_view message);

private:
    static LogLevel minLevel_;
};

// Trace/Debug compile out entirely in non-debug builds — zero cost, not just filtered at runtime.
#ifdef ENGINE_DEBUG
    #define LOG_TRACE(msg) Logger::Log(LogLevel::Trace, __FILE__, __LINE__, __func__, msg)
    #define LOG_DEBUG(msg) Logger::Log(LogLevel::Debug, __FILE__, __LINE__, __func__, msg)
#else
    #define LOG_TRACE(msg) ((void)0)
    #define LOG_DEBUG(msg) ((void)0)
#endif

#define LOG_INFO(msg)    Logger::Log(LogLevel::Info,    __FILE__, __LINE__, __func__, msg)
#define LOG_WARNING(msg) Logger::Log(LogLevel::Warning, __FILE__, __LINE__, __func__, msg)
#define LOG_ERROR(msg)   Logger::Log(LogLevel::Error,   __FILE__, __LINE__, __func__, msg)
#define LOG_FATAL(msg)   Logger::Log(LogLevel::Fatal,   __FILE__, __LINE__, __func__, msg)