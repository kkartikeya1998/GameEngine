#include "log/ConsoleSink.h"
#include <iostream>

void ConsoleSink::Write(LogLevel level, std::string_view formattedLine) {
    std::ostream& out = (level >= LogLevel::Error) ? std::cerr : std::cout;
    out << formattedLine << "\n";
    if (level == LogLevel::Fatal) out.flush();
}