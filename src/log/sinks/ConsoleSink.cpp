#include "log/ConsoleSink.h"
#include <iostream>
#include <format>
#include <filesystem>

namespace {
    std::string_view LevelName(LogLevel level) {
        switch (level) {
            case LogLevel::Trace:   return "TRACE";
            case LogLevel::Debug:   return "DEBUG";
            case LogLevel::Info:    return "INFO ";
            case LogLevel::Warning: return "WARN ";
            case LogLevel::Error:   return "ERROR";
            case LogLevel::Fatal:   return "FATAL";
        }
        return "UNKNOWN";
    }
}

void ConsoleSink::Write(const LogRecord& record) {
    std::ostream& out = (record.level >= LogLevel::Error) ? std::cerr : std::cout;

    auto time = std::chrono::current_zone()->to_local(record.timestamp);
    std::string filename = std::filesystem::path(record.location.file_name()).filename().string();

    // [HH:MM:SS.sss] [LEVEL] [FILE:LINE] (FUNC) - MESSAGE
    out << std::format("[{:%H:%M:%S}] [{}] [{}:{}] ({}) - {}\n",
                       time,
                       LevelName(record.level),
                       filename,
                       record.location.line(),
                       record.location.function_name(),
                       record.message);

    if (record.level == LogLevel::Fatal) {
        out.flush();
    }
}

void ConsoleSink::Flush() {
    std::cout.flush();
    std::cerr.flush();
}
