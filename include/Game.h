#pragma once

#include "system/GameController.h"
#include "render/SFMLRenderer.h"

class Game {
    public:
        Game();
        void run();
    private:
        GameController controller_;
        std::unique_ptr<IRenderer> renderer_;
        
};