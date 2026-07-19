#pragma once

#include "log/ILogSink.h"
#include <fstream>
#include <filesystem>

class FileSink : public ILogSink {
public:
    explicit FileSink(const std::filesystem::path& path);
    void Write(const LogRecord& record) override;
    void Flush() override;

private:
    std::ofstream file_;
};
