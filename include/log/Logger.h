#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <source_location>
#include "log/LogLevel.h"
#include "log/ILogSink.h"
#include "log/LogRecord.h"

/**
 * @brief Logger — static logging system.
 * Dispatches log records to registered sinks. Captures source location automatically.
 */
class Logger {
public:
    static void SetLevel(LogLevel level);
    static void AddSink(std::unique_ptr<ILogSink> sink);
    
    /**
     * @brief Core logging function.
     * @param level Importance of the message.
     * @param message The log message.
     * @param location Captured source location (file, line, function).
     */
    static void Log(LogLevel level, std::string_view message, 
                    std::source_location location = std::source_location::current());

    /**
     * @brief Flushes all registered sinks.
     */
    static void Flush();

private:
    static LogLevel minLevel_;
    static std::vector<std::unique_ptr<ILogSink>> sinks_;
    static std::mutex mutex_;
};

#ifdef ENGINE_DEBUG
    #define LOG_TRACE(msg) Logger::Log(LogLevel::Trace, msg)
    #define LOG_DEBUG(msg) Logger::Log(LogLevel::Debug, msg)
#else
    #define LOG_TRACE(msg) ((void)0)
    #define LOG_DEBUG(msg) ((void)0)
#endif

#define LOG_INFO(msg)    Logger::Log(LogLevel::Info,    msg)
#define LOG_WARNING(msg) Logger::Log(LogLevel::Warning, msg)
#define LOG_ERROR(msg)   Logger::Log(LogLevel::Error,   msg)
#define LOG_FATAL(msg)   Logger::Log(LogLevel::Fatal,   msg)
