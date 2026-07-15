#pragma once

#include "asset/AssetDatabase.h"
#include "asset/metadata/DirectionNaming.h"
#include "asset/metadata/PmdDirectionOrder.h"
#include "asset/metadata/PmdAnimationSetMetadata.h"

#include "entities/Registry.h"

#include "component/RenderComponent.h"
#include "component/DirectionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/WalkCycleTimer.h"
#include "component/AnimationPlayerComponent.h"

class AnimationSystem
{
public:
    explicit AnimationSystem(const AssetDatabase &assets)
        : assets_(assets)
    {
    }

    void update(Registry &registry, float dt)
    {

        for (EntityID id :
             registry.view<
                 AnimationPlayerComponent,
                 RenderComponent,
                 DirectionComponent>())
        {
            if (animateWithPmdPlayerIfAvailable(registry, id, dt))
                continue;

            animateWithPlayer(registry, id, dt);
        }


        for (EntityID id :
             registry.view<
                 WalkCycleTimer,
                 RenderComponent,
                 DirectionComponent,
                 MovementStateComponent>())
        {
            if (registry.has<AnimationPlayerComponent>(id))
                continue;

            if (animateWithPmdWalkCycleIfAvailable(registry, id))
                continue;

            animateWithWalkCycle(registry, id);
        }
    }

private:
    bool animateWithPmdPlayerIfAvailable(
        Registry &registry,
        EntityID id,
        float dt)
    {
        auto &render =
            *registry.get<RenderComponent>(id);

        const auto *set =
            assets_.findPmdAnimationSet(
                render.assetId);

        if (!set)
            return false;

        auto &player =
            *registry.get<AnimationPlayerComponent>(id);

        if (set->clips.find(player.currentAnimation) == set->clips.end())
        {
            return false;
        }

        animateWithPmdPlayer(
            registry,
            id,
            dt,
            *set);

        return true;
    }

    void animateWithPmdPlayer(
        Registry &registry,
        EntityID id,
        float dt,
        const PmdAnimationSetMetadata &set)
    {
        auto &render =
            *registry.get<RenderComponent>(id);

        auto &direction =
            *registry.get<DirectionComponent>(id);

        auto &player =
            *registry.get<AnimationPlayerComponent>(id);

        const auto &clip =
            set.clips.at(player.currentAnimation);

        if (player.playing && !player.finished)
        {
            player.elapsed += dt;

            while (player.elapsed >=
                   clip.frameDurations[player.currentFrame])
            {
                player.elapsed -=
                    clip.frameDurations[player.currentFrame];

                player.currentFrame++;

                if (player.currentFrame >= clip.frameCount)
                {
                    if (clip.loop)
                    {
                        player.currentFrame = 0;
                    }
                    else
                    {
                        player.currentFrame =
                            clip.frameCount - 1;

                        player.finished = true;
                        player.playing = false;
                        break;
                    }
                }
            }
        }

        render.name =
            render.assetId + "_" + player.currentAnimation + "_" + std::to_string(player.currentFrame);

        render.texturePath =
            clip.texturePath;

        render.textureRect =
            clip.frameRect(
                PmdDirectionOrder::rowFor(direction.facing),
                player.currentFrame);

        render.sourceTileSize =
            clip.frameWidth;
    }

    bool animateWithPmdWalkCycleIfAvailable(
        Registry &registry,
        EntityID id)
    {
        auto &render =
            *registry.get<RenderComponent>(id);

        const auto *set =
            assets_.findPmdAnimationSet(
                render.assetId);

        if (!set)
            return false;

        auto &movement =
            *registry.get<MovementStateComponent>(id);

        std::string animation;

        switch (movement.current)
        {
        case MovementState::Walking:
            animation = "walk";
            break;

        case MovementState::Sprinting:
            animation = "run";
            break;

        case MovementState::Idle:
        default:
            animation = "idle";
            break;
        }

        auto clip =
            set->clips.find(animation);

        if (clip == set->clips.end())
            return false;

        auto &direction =
            *registry.get<DirectionComponent>(id);

        auto &timer =
            *registry.get<WalkCycleTimer>(id);

        const auto &anim =
            clip->second;

        int frame =
            static_cast<int>(
                timer.getProgress() * anim.frameCount) %
            anim.frameCount;

        render.name =
            render.assetId + "_" + animation + "_" + std::to_string(frame);

        render.texturePath =
            anim.texturePath;

        render.textureRect =
            anim.frameRect(
                PmdDirectionOrder::rowFor(direction.facing),
                frame);

        render.sourceTileSize =
            anim.frameWidth;

        return true;
    }

    void animateWithPlayer(
        Registry &registry,
        EntityID id,
        float dt)
    {
        auto &render =
            *registry.get<RenderComponent>(id);

        auto &direction =
            *registry.get<DirectionComponent>(id);

        auto &player =
            *registry.get<AnimationPlayerComponent>(id);

        const auto *anim =
            assets_.findAnimation(
                render.assetId + "_" + player.currentAnimation);

        if (!anim)
            return;

        std::string dir =
            toString(direction.facing);

        int frameCount = 1;

        auto it =
            anim->directions.find(dir);

        if (it != anim->directions.end())
            frameCount =
                it->second.frameCount;

        if (player.playing && !player.finished)
        {
            player.elapsed += dt;

            while (player.elapsed >=
                   anim->frameDuration)
            {
                player.elapsed -=
                    anim->frameDuration;

                player.currentFrame++;

                if (player.currentFrame >= frameCount)
                {
                    if (anim->loop)
                        player.currentFrame = 0;

                    else
                    {
                        player.currentFrame =
                            frameCount - 1;

                        player.finished = true;
                        player.playing = false;
                    }
                }
            }
        }

        render.name =
            render.assetId + "_" + player.currentAnimation + "_" + dir + "_" + std::to_string(player.currentFrame);

        render.texturePath =
            anim->texturePath;

        render.textureRect =
            anim->frameRect(
                dir,
                player.currentFrame);

        render.sourceTileSize =
            anim->frameWidth;
    }

    void animateWithWalkCycle(
        Registry &registry,
        EntityID id)
    {
        auto &render =
            *registry.get<RenderComponent>(id);

        auto &movement =
            *registry.get<MovementStateComponent>(id);

        auto &direction =
            *registry.get<DirectionComponent>(id);

        auto &timer =
            *registry.get<WalkCycleTimer>(id);

        std::string state =
            toString(movement.current);

        auto *anim =
            assets_.findAnimation(
                render.assetId + "_" + state);

        if (!anim)
        {
            anim =
                assets_.findAnimation(
                    render.assetId + "_walk");
        }

        if (!anim)
            return;

        std::string dir =
            toString(direction.facing);

        auto dirIt =
            anim->directions.find(dir);

        int frames =
            dirIt != anim->directions.end()
                ? dirIt->second.frameCount
                : 1;

        int frame =
            static_cast<int>(
                timer.getProgress() * frames) %
            frames;

        render.name =
            render.assetId + "_" + state + "_" + dir + "_" + std::to_string(frame);

        render.texturePath =
            anim->texturePath;

        render.textureRect =
            anim->frameRect(
                dir,
                frame);

        render.sourceTileSize =
            anim->frameWidth;
    }

private:
    const AssetDatabase &assets_;
};