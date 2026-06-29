#include "render/RenderSystem.h"
#include "system/RenderStateSystem.h"
#include "system/MovementSystem.h"
#include "system/MapObjectSystem.h"
#include "entities/movement/GridMovementComponent.h"
#include "entities/movement/FreeMovementComponent.h"
#include "render/GridRenderComponent.h"
#include "render/FreeRenderComponent.h"
#include "world/MapObjectRenderComponent.h"
#include <optional>
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

namespace {
    struct Renderable {
        float depthY;
        std::function<void()> draw;
    };
}

RenderSystem::RenderSystem(std::unique_ptr<IRenderer> renderer)
    : renderer_(std::move(renderer))
{
    if (!renderer_) {
        throw std::invalid_argument("RenderSystem: renderer cannot be null");
    }
}

void RenderSystem::render(GameController& controller, float dt)
{
    renderer_->clear();

    World* world = controller.getWorld();
    Entity* player = controller.getPlayer();

    if (!world || !player) {
        renderer_->present();
        return;
    }

    Map* map = world->getActiveMap();

    if (map) {
        for (int y = 0; y < map->getHeight(); ++y) {
            for (int x = 0; x < map->getWidth(); ++x) {
                renderer_->drawTile(x, y, map->tile_at(x, y).typeName());
            }
        }

        std::vector<Renderable> renderables;
        renderables.reserve(map->getMapObjects().size() + map->getNpcs().size() + 1);

        // ---- Map objects --------------------------------------------
        // Restored to match the original logic exactly, now that
        // MapObjectSystem::getCollisionBox is a verified port of
        // MapObject::getCollisionBox (see MapObjectSystem.cpp —
        // numerically confirmed identical to the original by test).
        // depthY uses the collision box's bottom edge when present,
        // falling back to raw originY otherwise — same as before this
        // ECS pass ever started.
        for (const auto& obj : map->getMapObjects()) {
            const auto* render = obj->get<MapObjectRenderComponent>();
            if (!render) continue;

            std::optional<AABB> collisionBox = MapObjectSystem::getCollisionBox(*render);

            if (collisionBox) {
                renderer_->drawDebugRect(collisionBox->x, collisionBox->y, collisionBox->width, collisionBox->height);
            }

            float depthY = collisionBox
                ? (collisionBox->y + collisionBox->height)
                : static_cast<float>(MapObjectSystem::getOriginPixelY(*render));

            float originX = static_cast<float>(MapObjectSystem::getOriginPixelX(*render));
            float originY = static_cast<float>(MapObjectSystem::getOriginPixelY(*render));
            std::string typeName = MapObjectSystem::getTypeName(*render);

            renderables.push_back(Renderable{
                depthY,
                [this, originX, originY, typeName]() {
                    renderer_->drawMapObject(originX, originY, typeName);
                }
            });
        }

        // ---- NPCs -----------------------------------------------------
        // Dispatches on which movement/render component pair an NPC
        // Entity actually has, mirroring how Player is handled below.
        // Both branches step render state inline (the chosen tradeoff:
        // render() mutates lerp/walk-cycle state as a side effect of
        // drawing).
        // for (const auto& npcEntity : map->getNpcs()) {
        //     if (auto* grid = npcEntity->get<GridMovementComponent>()) {
        //         auto* gridRender = npcEntity->get<GridRenderComponent>();
        //         if (!gridRender) continue;

        //         RenderStateSystem::stepGridRender(
        //             *gridRender, dt,
        //             static_cast<float>(grid->gridX), static_cast<float>(grid->gridY));

        //         float renderX = gridRender->renderX;
        //         float renderY = gridRender->renderY;
        //         Direction facing = grid->facing;
        //         float progress = gridRender->progress;

        //         renderables.push_back(Renderable{
        //             renderY,
        //             [this, renderX, renderY, facing, progress]() {
        //                 renderer_->drawPlayer(renderX, renderY, facing, progress);
        //             }
        //         });
        //     } else if (auto* free = npcEntity->get<FreeMovementComponent>()) {
        //         auto* freeRender = npcEntity->get<FreeRenderComponent>();
        //         if (!freeRender) continue;

        //         RenderStateSystem::stepFreeRender(
        //             *freeRender, dt,
        //             free->x, free->y, free->isMoving);

        //         float renderX = freeRender->renderX;
        //         float renderY = freeRender->renderY;
        //         Direction facing = free->facing;
        //         float progress = RenderStateSystem::getFreeProgress(*freeRender);

        //         renderables.push_back(Renderable{
        //             renderY,
        //             [this, renderX, renderY, facing, progress]() {
        //                 renderer_->drawPlayer(renderX, renderY, facing, progress);
        //             }
        //         });
        //     }
        // }

        // ---- Player -----------------------------------------------------
        // Player is a FreeMovementComponent + FreeRenderComponent
        // entity, same as before (makePlayer only ever built
        // FreeMovementMechanics) — see Player.h.
        
        auto* playerPos = player->get<PositionComponent>();
        auto* playerMove = player->get<FreeMovementComponent>();
        auto* playerRender = player->get<FreeRenderComponent>();

        if (playerPos && playerMove && playerRender) {
            AABB playerBox = MovementSystem::getFreeHitbox(*playerPos, *playerMove);
            renderer_->drawDebugRect(playerBox.x, playerBox.y, playerBox.width, playerBox.height);

            RenderStateSystem::stepFreeRender(
                *playerRender, dt,
                playerPos->x, playerPos->y, playerMove->isMoving);

            float renderX = playerRender->renderX;
            float renderY = playerRender->renderY;
            Direction facing = playerPos->facing;
            float progress = RenderStateSystem::getFreeProgress(*playerRender);

            renderables.push_back(Renderable{
                playerBox.y + playerBox.height,
                [this, renderX, renderY, facing, progress]() {
                    renderer_->drawPlayer(renderX, renderY, facing, progress);
                }
            });
        }

        std::sort(renderables.begin(), renderables.end(),
                  [](const Renderable& a, const Renderable& b) {
                      return a.depthY < b.depthY;
                  });

        for (auto& r : renderables) {
            r.draw();
        }
    } else {
        auto* playerPos = player->get<PositionComponent>();
        auto* playerMove = player->get<FreeMovementComponent>();
        auto* playerRender = player->get<FreeRenderComponent>();
        if (playerPos && playerMove && playerRender) {
            renderer_->drawPlayer(
                playerRender->renderX,
                playerRender->renderY,
                playerPos->facing,
                RenderStateSystem::getFreeProgress(*playerRender)
            );
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