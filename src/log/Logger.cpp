#include "log/Logger.h"

#include <cstring>

LogLevel Logger::minLevel_ =
#ifdef ENGINE_DEBUG
    LogLevel::Trace;
#else
    LogLevel::Warning;
#endif

std::vector<std::unique_ptr<ILogSink>> Logger::sinks_;

namespace {
    const char* LevelName(LogLevel level) {
        switch (level) {
            case LogLevel::Trace:   return "TRACE";
            case LogLevel::Debug:   return "DEBUG";
            case LogLevel::Info:    return "INFO";
            case LogLevel::Warning: return "WARN";
            case LogLevel::Error:   return "ERROR";
            case LogLevel::Fatal:   return "FATAL";
        }
        return "?";
    }

    const char* FileName(const char* path) {
        const char* slash = std::strrchr(path, '/');
        const char* backslash = std::strrchr(path, '\\');
        const char* last = (slash > backslash) ? slash : backslash;
        return last ? last + 1 : path;
    }
}

void Logger::SetLevel(LogLevel level) {
    minLevel_ = level;
}

void Logger::AddSink(std::unique_ptr<ILogSink> sink) {
    sinks_.push_back(std::move(sink));
}

void Logger::Log(LogLevel level, const char* file, int line, const char* func, std::string_view message) {
    if (level < minLevel_) return;

    std::string formatted = std::string("[") + LevelName(level) + "] " +
                             FileName(file) + ":" + std::to_string(line) +
                             " (" + func + ") - " + std::string(message);

    for (auto& sink : sinks_) {
        sink->Write(level, formatted);
    }
}