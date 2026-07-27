#pragma once

#include <string_view>
#include <source_location>
#include <chrono>
#include "engine/logging/LogLevel.h"

/**
 * @brief Represents a single log entry with all its metadata.
 */
struct LogRecord {
    LogLevel level;
    std::string_view message;
    std::source_location location;
    std::chrono::system_clock::time_point timestamp;
    uint64_t frameNumber;
    
    // Potential future metadata can be added here without breaking existing sinks

    // std::thread::id threadId;
    // std::string_view subsystem;
};
