#pragma once

#include "asset/AssetDatabase.h"
#include "asset/metadata/DirectionNaming.h"
#include "entities/Registry.h"
#include "component/RenderComponent.h"
#include "component/DirectionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/WalkCycleTimer.h"
#include "component/AnimationPlayerComponent.h"

class AnimationSystem
{
public:
    explicit AnimationSystem(const AssetDatabase& assets) : assets_(assets) {}

    void update(Registry& registry, float dt)
    {
        // AnimationPlayerComponent takes priority over WalkCycleTimer when
        // both are present — explicit animation state wins over inferred.
        for (EntityID id : registry.view<AnimationPlayerComponent, RenderComponent, DirectionComponent>())
            animateWithPlayer(registry, id, dt);

        for (EntityID id : registry.view<WalkCycleTimer, RenderComponent, DirectionComponent, MovementStateComponent>())
        {
            if (registry.has<AnimationPlayerComponent>(id))
                continue;
            animateWithWalkCycle(registry, id);
        }
    }

private:
    void animateWithPlayer(Registry& registry, EntityID id, float dt)
    {
        auto& render = *registry.get<RenderComponent>(id);
        auto& direction = *registry.get<DirectionComponent>(id);
        auto& player = *registry.get<AnimationPlayerComponent>(id);

        std::string characterName = render.name.substr(0, render.name.find('_'));
        const AnimationAssetMetadata* anim = assets_.findAnimation(characterName + "_" + player.currentAnimation);
        if (!anim)
            return;

        const std::string dirStr = toString(direction.facing);
        auto dirIt = anim->directions.find(dirStr);
        int frameCount = (dirIt != anim->directions.end()) ? dirIt->second.frameCount : 1;

        if (player.playing && !player.finished && anim->frameDuration > 0.f)
        {
            player.elapsed += dt;
            while (player.elapsed >= anim->frameDuration)
            {
                player.elapsed -= anim->frameDuration;
                player.currentFrame++;
                if (player.currentFrame >= frameCount)
                {
                    if (anim->loop)
                    {
                        player.currentFrame = 0;
                    }
                    else
                    {
                        player.currentFrame = frameCount - 1;
                        player.finished = true;
                        player.playing = false;
                        break;
                    }
                }
            }
        }

        render.name = characterName + "_" + player.currentAnimation + "_" + dirStr + "_" + std::to_string(player.currentFrame);
        render.texturePath = anim->texturePath;
        render.textureRect = anim->frameRect(dirStr, player.currentFrame);
        render.sourceTileSize = anim->frameWidth;
    }

    void animateWithWalkCycle(Registry& registry, EntityID id)
    {
        auto& render = *registry.get<RenderComponent>(id);
        auto& direction = *registry.get<DirectionComponent>(id);
        auto& movementState = *registry.get<MovementStateComponent>(id);
        auto& walkCycle = *registry.get<WalkCycleTimer>(id);

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
        render.sourceTileSize = anim->frameWidth;
    }

    const AssetDatabase& assets_;
};