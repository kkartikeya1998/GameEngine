#include "Game.h"
#include "engine/logging/Logger.h"
#include "engine/logging/ConsoleSink.h"
#include "engine/logging/FileSink.h"

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