#pragma once
#include "log/ILogSink.h"

class ConsoleSink : public ILogSink {
public:
    void Write(LogLevel level, std::string_view formattedLine) override;
};