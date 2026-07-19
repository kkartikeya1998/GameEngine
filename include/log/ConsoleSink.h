#pragma once
#include "log/ILogSink.h"

class ConsoleSink : public ILogSink {
public:
    void Write(const LogRecord& record) override;
    void Flush() override;
};
