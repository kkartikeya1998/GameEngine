#include "log/Logger.h"

LogLevel Logger::minLevel_ =
#ifdef ENGINE_DEBUG
    LogLevel::Trace;
#else
    LogLevel::Warning;
#endif

std::vector<std::unique_ptr<ILogSink>> Logger::sinks_;
std::mutex Logger::mutex_;

void Logger::SetLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    minLevel_ = level;
}

void Logger::AddSink(std::unique_ptr<ILogSink> sink) {
    std::lock_guard<std::mutex> lock(mutex_);
    sinks_.push_back(std::move(sink));
}

void Logger::Log(LogLevel level, std::string_view message, std::source_location location) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (level < minLevel_) return;
    }

    LogRecord record {
        .level = level,
        .message = message,
        .location = location,
        .timestamp = std::chrono::system_clock::now()
    };

    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& sink : sinks_) {
        sink->Write(record);
    }
}

void Logger::Flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& sink : sinks_) {
        sink->Flush();
    }
}
