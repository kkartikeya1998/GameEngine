#include "Game.h"

#include <iostream>
#include <memory>
#include <stdexcept>

int main() {
    try {
        Game game;
        game.Run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
        return 1;
    }
}