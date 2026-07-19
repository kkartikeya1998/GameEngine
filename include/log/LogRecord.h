#pragma once

#include <string_view>
#include <source_location>
#include <chrono>
#include "log/LogLevel.h"

/**
 * @brief Represents a single log entry with all its metadata.
 */
struct LogRecord {
    LogLevel level;
    std::string_view message;
    std::source_location location;
    std::chrono::system_clock::time_point timestamp;

    // Potential future metadata can be added here without breaking existing sinks

    // std::thread::id threadId;
    // uint64_t frameNumber;
    // std::string_view subsystem;
};
