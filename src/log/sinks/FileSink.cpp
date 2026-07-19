#include "log/FileSink.h"
#include <format>
#include <chrono>
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

FileSink::FileSink(const std::filesystem::path& path)
    : file_(path, std::ios::out | std::ios::app) {
}

void FileSink::Write(const LogRecord& record) {
    if (!file_.is_open()) return;

    auto time = std::chrono::current_zone()->to_local(record.timestamp);
    std::string filename = std::filesystem::path(record.location.file_name()).filename().string();

    // [YYYY-MM-DD HH:MM:SS.sss] [LEVEL] [FILE:LINE] (FUNC) - MESSAGE
    file_ << std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] [{}:{}] ({}) - {}\n",
                         time,
                         LevelName(record.level),
                         filename,
                         record.location.line(),
                         record.location.function_name(),
                         record.message);

    if (record.level >= LogLevel::Error) {
        file_.flush();
    }
}

void FileSink::Flush() {
    file_.flush();
}
