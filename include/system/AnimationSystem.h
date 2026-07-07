#pragma once

#include "asset/repositories/CharacterRepository.h"
#include "asset/metadata/DirectionNaming.h"
#include "tmp/component/RenderComponent.h"
#include "tmp/component/DirectionComponent.h"
#include "tmp/component/MovementStateComponent.h"
#include "tmp/component/WalkCycleTimer.h"
#include "system/GameController.h"

#include <iostream>

class AnimationSystem
{
public:
    explicit AnimationSystem(const CharacterRepository &characters) : characters_(characters) {}

    void update(GameController& controller, float dt)
    {
        for (Entity *e : controller.getWorld()->view<WalkCycleTimer>())
            animate(*e);

        animate(*controller.getPlayer());
    }

private:
    void animate(Entity &e)
    {
        // For now we only animate entities that have all of these components; so basically characters 
        if (!e.has<RenderComponent>() || !e.has<DirectionComponent>() ||
            !e.has<MovementStateComponent>() || !e.has<WalkCycleTimer>())
            return;

        auto &render = *e.get<RenderComponent>();
        const auto &direction = *e.get<DirectionComponent>();
        const auto &movementState = *e.get<MovementStateComponent>();
        const auto &walkCycle = *e.get<WalkCycleTimer>();

        const std::string dirStr = toString(direction.facing);
        std::string state = toString(movementState.current);

        std::string characterName = render.name.substr(0, render.name.find('_')); // extract 
        int frames = characters_.frameCount(characterName, state, dirStr);
        if (frames <= 0)
        {
            state = "walk";
            frames = characters_.frameCount(characterName, state, dirStr);
        }
        if (frames <= 0)
            return;

        int frameIndex = static_cast<int>(walkCycle.getProgress() * frames) % frames;
        
        std::string key = characterName + "_" + state + "_" + dirStr + "_" + std::to_string(frameIndex);

        render.name = key;
        if (const auto *meta = characters_.find(key))
        {
            render.texturePath = meta->renderBox.texturePath;
            render.textureRect = meta->renderBox.textureRect;
            render.sourceTileSize = meta->renderBox.sourceTileSize;
        }
    }

    const CharacterRepository &characters_;
};