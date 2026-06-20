#pragma once

#include "system/GameController.h"
#include "render/RenderSystem.h"
#include "render/SpriteAtlas.h"

class Game {
    public:
        Game();
        void run();
    private:
        GameController controller_;
        std::unique_ptr<SpriteAtlas> atlas_;
        std::unique_ptr<RenderSystem> renderSystem_;
        
};