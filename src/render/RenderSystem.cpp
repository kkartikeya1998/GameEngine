#include "render/RenderSystem.h"
#include "system/MovementSystem.h"
#include "tmp/movement/FreeMovementComponent.h"
#include "tmp/movement/RenderComponent.h"
#include "tmp/movement/CollisionComponent.h"
#include <optional>
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

namespace
{
    struct Renderable
    {
        float depthY;
        std::function<void()> draw;
    };
}

RenderSystem::RenderSystem(std::unique_ptr<IRenderer> renderer)
    : renderer_(std::move(renderer))
{
    if (!renderer_)
    {
        throw std::invalid_argument("RenderSystem: renderer cannot be null");
    }
}

void RenderSystem::render(GameController &controller, float dt)
{
    renderer_->clear();

    World *world = controller.getWorld();
    Entity *player = controller.getPlayer();

    if (!world || !player)
    {
        renderer_->present();
        return;
    }

    Map *map = world->getActiveMap();

    if (map)
    {
        for (int y = 0; y < map->getHeight(); ++y)
        {
            for (int x = 0; x < map->getWidth(); ++x)
            {
                renderer_->drawTile(x, y, map->tile_at(x, y).getRenderComponent());
            }
        }

        std::vector<Renderable> renderables;
        renderables.reserve(map->getMapObjects().size() + map->getNpcs().size() + 1);


        for (const auto &mapObj : map->getMapObjects())
        {
            const auto *mapObjRender = mapObj->get<RenderComponent>();
            const auto *mapObjPos = mapObj->get<PositionComponent>();
            if (!mapObjRender || !mapObjPos)
                continue;

            const auto *mapObjCollision = mapObj->get<CollisionComponent>();
            
            if (mapObjCollision)
            {
                auto debugRect = mapObjCollision->resolve(mapObjPos->x, mapObjPos->y);
                renderer_->drawDebugRect(debugRect.x, debugRect.y, debugRect.width, debugRect.height);
            }

            float depthY = mapObjCollision
                               ? (mapObjCollision->resolve(mapObjPos->x, mapObjPos->y).y + mapObjCollision->resolve(mapObjPos->x, mapObjPos->y).height)
                               : static_cast<float>(mapObjPos->y);


            renderables.push_back(Renderable{
                depthY,
                [this, mapObjPos = *mapObjPos, mapObjRender = *mapObjRender]()
                {
                    renderer_->drawMapObject(mapObjRender);
                }});
        }

        // ---- Player -----------------------------------------------------
        // Player is a FreeMovementComponent + RenderComponent
        // entity, same as before (makePlayer only ever built
        // FreeMovementMechanics) — see Player.h.

        const auto *playerPos = player->get<PositionComponent>();
        const auto *playerMove = player->get<FreeMovementComponent>();
        const auto *playerRender = player->get<RenderComponent>();
        const auto *playerDirection = player->get<DirectionComponent>();
        const auto *playerTimer = player->get<WalkCycleTimer>();

        if (playerPos && playerMove && playerRender && playerDirection)
        {
            AABB playerBox = MovementSystem::getFreeHitbox(*playerPos, *playerMove);
            renderer_->drawDebugRect(playerBox.x, playerBox.y, playerBox.width, playerBox.height);

            float animProgress = playerTimer ? playerTimer->getProgress() : 0.f;

            renderables.push_back(Renderable{
                playerBox.y + playerBox.height,
                [this, playerPos = *playerPos, playerDirection = *playerDirection,
                playerRender = *playerRender, animProgress]()
                {
                    renderer_->drawPlayer(playerPos, playerDirection, playerRender, animProgress);
                }});
        }

        std::sort(renderables.begin(), renderables.end(),
                  [](const Renderable &a, const Renderable &b)
                  {
                      return a.depthY < b.depthY;
                  });

        for (auto &r : renderables)
        {
            r.draw();
        }
    }
    else
    {
        const auto *playerPos = player->get<PositionComponent>();
        const auto *playerMove = player->get<FreeMovementComponent>();
        const auto *playerRender = player->get<RenderComponent>();
        const auto *playerDir = player->get<DirectionComponent>();
        if (playerPos && playerMove && playerRender)
        {
            renderer_->drawPlayer(
                *playerPos,*playerDir,
                *playerRender,
                0.f); // fix
        }
    }

    renderer_->present();
}

bool RenderSystem::isOpen() const
{
    return renderer_->isOpen();
}

std::optional<sf::Event> RenderSystem::pollEvent()
{
    return renderer_->pollEvent();
}