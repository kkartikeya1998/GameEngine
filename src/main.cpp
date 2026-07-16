#include "Game.h"
#include "utils/Logger.h"

#include <stdexcept>

int main() {
    try {
        Game game;
        game.Run();
        return 0;
    } catch (const std::exception& e) {
        LOG_FATAL(e.what());
        return 1;
    }
}