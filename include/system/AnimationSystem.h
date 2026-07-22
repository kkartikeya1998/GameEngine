#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_set>

#include "asset/AssetDatabase.h"
#include "asset/metadata/DirectionNaming.h"
#include "asset/metadata/PmdDirectionOrder.h"

#include "entities/Registry.h"

#include "component/SpriteFrameComponent.h"
#include "component/DirectionComponent.h"
#include "component/AnimationComponent.h"

#include "system/MovementAnimationSystem.h"

#include "log/Logger.h"

class AnimationSystem
{
public:
    explicit AnimationSystem(const AssetDatabase &assets) : assets_(assets) {}

    void update(Registry &registry, float dt)
    {
        MovementAnimationSystem::update(registry);
        for (EntityID id : registry.view<AnimationComponent, SpriteFrameComponent, DirectionComponent>())
        {
            auto &frame = *registry.get<SpriteFrameComponent>(id);
            auto &player = *registry.get<AnimationComponent>(id);
            auto &direction = *registry.get<DirectionComponent>(id);

            // resolveClip now needs assetId directly rather than via a RenderComponent:
            auto clip = resolveClip(*registry.get<SpriteAssetComponent>(id), player.currentAnimation);
            if (!clip)
            {
                const std::string &assetId = registry.get<SpriteAssetComponent>(id)->assetId;
                std::string warnKey = assetId + "_" + player.currentAnimation;
                if (warnedMissingClips_.insert(warnKey).second)
                {
                    LOG_WARNING("AnimationSystem: no clip '" + player.currentAnimation +
                                "' for asset '" + assetId + "' — entity frame will freeze until this is fixed");
                }
                continue;
            }

            advancePlayer(player, dt, *clip, direction.facing);
            writeSpriteFrame(frame, player, *clip, direction.facing);
        }
    }

private:
    // A clip reduced to the shape both PMD and legacy formats share.
    // frameCountAt/frameRectAt take Direction because legacy clips can (in
    // principle) have a different frame count per direction; PMD's
    // constant-per-row clips just ignore the argument.
    struct ResolvedClip
    {
        std::string texturePath;
        int frameWidth = 0;
        bool loop = true;
        std::function<int(Direction)> frameCountAt;
        std::function<float(int frame)> frameDurationAt;
        std::function<sf::IntRect(Direction, int frame)> frameRectAt;
    };

    const AssetDatabase &assets_;
    std::unordered_set<std::string> warnedMissingClips_;

    std::optional<ResolvedClip> resolveClip(const SpriteAssetComponent &render, const std::string &animName) const
    {
        if (const auto *set = assets_.findPmdAnimationSet(render.assetId))
        {
            auto it = set->clips.find(animName);
            if (it != set->clips.end())
            {
                const auto &clip = it->second;
                ResolvedClip resolved;
                resolved.texturePath = clip.texturePath;
                resolved.frameWidth = clip.frameWidth;
                resolved.loop = clip.loop;
                resolved.frameCountAt = [&clip](Direction)
                { return clip.frameCount; };
                resolved.frameDurationAt = [&clip](int frame)
                { return clip.frameDurations[frame]; };
                resolved.frameRectAt = [&clip](Direction dir, int frame)
                {
                    return clip.frameRect(PmdDirectionOrder::rowFor(dir), frame);
                };
                return resolved;
            }
        }

        if (const auto *anim = assets_.findAnimation(render.assetId + "_" + animName))
        {
            ResolvedClip resolved;
            resolved.texturePath = anim->texturePath;
            resolved.frameWidth = anim->frameWidth;
            resolved.loop = anim->loop;
            resolved.frameCountAt = [anim](Direction dir)
            {
                auto it = anim->directions.find(toString(dir));
                return it != anim->directions.end() ? it->second.frameCount : 1;
            };
            resolved.frameDurationAt = [anim](int)
            { return anim->frameDuration; };
            resolved.frameRectAt = [anim](Direction dir, int frame)
            {
                return anim->frameRect(toString(dir), frame);
            };
            return resolved;
        }

        return std::nullopt;
    }

    void advancePlayer(AnimationComponent &player, float dt, const ResolvedClip &clip, Direction facing) const
    {
        if (!player.playing || player.finished)
            return;

        int frameCount = clip.frameCountAt(facing);
        player.elapsed += dt;

        while (player.elapsed >= clip.frameDurationAt(player.currentFrame))
        {
            player.elapsed -= clip.frameDurationAt(player.currentFrame);
            player.currentFrame++;

            if (player.currentFrame >= frameCount)
            {
                if (clip.loop)
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

    // resolveClip's signature: const RenderComponent& → const SpriteAssetComponent& (reads .assetId instead of .assetId — same field, different owner)
    // writeRenderFrame renamed writeSpriteFrame, writes into SpriteFrameComponent& instead of RenderComponent&:
    void writeSpriteFrame(SpriteFrameComponent &frame, const AnimationComponent &player,
                          const ResolvedClip &clip, Direction facing) const
    {
        frame.name = /* assetId */ "_" + player.currentAnimation + "_" + toString(facing) + "_" + std::to_string(player.currentFrame);
        // LOG_FATAL("Writing Sprite Frame: " + frame.name + "_" + player.currentAnimation + "_" + toString(facing) + "_" + std::to_string(player.currentFrame));
        // note: assetId is no longer on this struct — resolveClip needs to thread it through,
        // or writeSpriteFrame takes assetId as an extra parameter. Flagging: needs one more
        // pass to wire this cleanly once ResolvedClip carries the source assetId itself.
        frame.texturePath = clip.texturePath;
        frame.textureRect = clip.frameRectAt(facing, player.currentFrame);
        frame.sourceTileSize = clip.frameWidth;
    }
};