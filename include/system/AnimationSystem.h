#pragma once

#include "asset/AssetDatabase.h"
#include "asset/metadata/DirectionNaming.h"
#include "component/RenderComponent.h"
#include "component/DirectionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/WalkCycleTimer.h"
#include "system/GameController.h"

class AnimationSystem
{
public:
    explicit AnimationSystem(const AssetDatabase& assets) : assets_(assets) {}

    void update(GameController& controller, float dt)
    {
        for (Entity *e : controller.getWorld()->view<WalkCycleTimer>())
            animate(*e);
        animate(*controller.getPlayer());
    }

private:
    void animate(Entity &e)
    {
        if (!e.has<RenderComponent>() || !e.has<DirectionComponent>() ||
            !e.has<MovementStateComponent>() || !e.has<WalkCycleTimer>())
            return;

        auto &render = *e.get<RenderComponent>();
        const auto &direction = *e.get<DirectionComponent>();
        const auto &movementState = *e.get<MovementStateComponent>();
        const auto &walkCycle = *e.get<WalkCycleTimer>();

        const std::string dirStr = toString(direction.facing);
        std::string state = toString(movementState.current);
        std::string characterName = render.name.substr(0, render.name.find('_'));

        const AnimationAssetMetadata* anim = assets_.findAnimation(characterName + "_" + state);
        if (!anim) {
            state = "walk";
            anim = assets_.findAnimation(characterName + "_" + state);
        }
        if (!anim)
            return;

        auto dirIt = anim->directions.find(dirStr);
        int frameCount = (dirIt != anim->directions.end()) ? dirIt->second.frameCount : 1;
        int frameIndex = static_cast<int>(walkCycle.getProgress() * frameCount) % frameCount;

        render.name = characterName + "_" + state + "_" + dirStr + "_" + std::to_string(frameIndex);
        render.texturePath = anim->texturePath;
        render.textureRect = anim->frameRect(dirStr, frameIndex);
        render.sourceTileSize = anim->frameWidth; // grid sheets: square frames assumed
    }

    const AssetDatabase& assets_;
};