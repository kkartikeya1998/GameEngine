#include "engine/logging/Logger.h"

LogLevel Logger::minLevel_ =
#ifdef ENGINE_DEBUG
    LogLevel::Trace;
#else
    LogLevel::Warning;
#endif

std::vector<std::unique_ptr<ILogSink>> Logger::sinks_;
std::mutex Logger::mutex_;
uint64_t Logger::currentFrame_ = 0;

void Logger::Log(LogLevel level, std::string_view message, std::source_location location)
{
    uint64_t frame;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (level < minLevel_)
            return;
        frame = currentFrame_;
    }

    LogRecord record{
        .level = level,
        .message = message,
        .location = location,
        .timestamp = std::chrono::system_clock::now(),
        .frameNumber = frame};

    std::lock_guard<std::mutex> lock(mutex_);
    for (auto &sink : sinks_)
    {
        sink->Write(record);
    }
}

void Logger::SetFrameNumber(uint64_t frame)
{
    std::lock_guard<std::mutex> lock(mutex_);
    currentFrame_ = frame;
}

void Logger::SetLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(mutex_);
    minLevel_ = level;
}

void Logger::AddSink(std::unique_ptr<ILogSink> sink)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sinks_.push_back(std::move(sink));
}

void Logger::Flush()
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto &sink : sinks_)
    {
        sink->Flush();
    }
}
