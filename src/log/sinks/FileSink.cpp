#include "log/FileSink.h"

FileSink::FileSink(const std::filesystem::path &path)
    : file_(path, std::ios::out | std::ios::app)
{
}

void FileSink::Write(LogLevel level, std::string_view formattedLine)
{
    file_ << formattedLine << "\n";
    if (level == LogLevel::Fatal)
        file_.flush();
}