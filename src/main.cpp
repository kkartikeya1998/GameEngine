#include "Game.h"
#include "log/Logger.h"
#include "log/ConsoleSink.h"
#include "log/FileSink.h"

#include <stdexcept>

int main()
{
    Logger::AddSink(std::make_unique<ConsoleSink>());
    Logger::AddSink(std::make_unique<FileSink>(std::filesystem::path(PROJECT_ROOT) / "game.log"));
    try
    {
        Game game;
        game.Run();
        return 0;
    }
    catch (const std::exception &e)
    {
        LOG_FATAL(e.what());
        return 1;
    }
}