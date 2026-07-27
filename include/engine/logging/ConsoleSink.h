#pragma once
#include "engine/logging/ILogSink.h"

class ConsoleSink : public ILogSink {
public:
    void Write(const LogRecord& record) override;
    void Flush() override;
};
