#include "utils/Logger.h"

#include <cstring>
#include <iostream>

// Debug builds show everything by default; release builds only Warning and up.
LogLevel Logger::minLevel_ =
#ifdef ENGINE_DEBUG
    LogLevel::Trace;
#else
    LogLevel::Warning;
#endif

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

    // Strip directory prefix so logs show "MapLoader.cpp:42" not a full path.
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

void Logger::Log(LogLevel level, const char* file, int line, const char* func, std::string_view message) {
    if (level < minLevel_) return;

    std::ostream& out = (level >= LogLevel::Error) ? std::cerr : std::cout;
    out << "[" << LevelName(level) << "] " << FileName(file) << ":" << line
        << " (" << func << ") - " << message << "\n";

    if (level == LogLevel::Fatal) out.flush();
}