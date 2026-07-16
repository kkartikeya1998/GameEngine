#pragma once

#include <string_view>
#include "log/LogLevel.h"

// ---------------------------------------------------------------------------
// ILogSink — output target for formatted log lines. Logger owns a list of
// these and writes every accepted line to all of them.
// ---------------------------------------------------------------------------
class ILogSink {
public:
    virtual ~ILogSink() = default;
    virtual void Write(LogLevel level, std::string_view formattedLine) = 0;
};