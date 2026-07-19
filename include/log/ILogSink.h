#pragma once

#include "log/LogRecord.h"

/**
 * @brief ILogSink — output target for log records.
 * Logger owns a list of these and writes every accepted record to all of them.
 */
class ILogSink {
public:
    virtual ~ILogSink() = default;

    /**
     * @brief Writes a log record to the sink.
     * @param record The log record containing level, message, and metadata.
     */
    virtual void Write(const LogRecord& record) = 0;

    /**
     * @brief Flushes any buffered output in the sink.
     */
    virtual void Flush() {}
};
