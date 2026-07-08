#include "render/RenderSystem.h"
#include "system/MovementSystem.h"
#include "tmp/component/FreeMovementComponent.h"
#include "tmp/component/RenderComponent.h"
#include "tmp/component/CollisionComponent.h"
#include <optional>
#include <algorithm>
#include <vector>
#include <functional>

namespace
{
    struct Renderable
    {
        float depthY;
        std::function<void()> draw;
    };
}

RenderSystem::RenderSystem(std::unique_ptr<IRenderer> renderer,
                            const TileRepository& tileRepository,
                            const std::filesystem::path& tileSpritesheetPath)
    : renderer_(std::move(renderer))
    , tileAtlas_(tileSpritesheetPath, tileRepository)
    , tileTexturePath_(tileSpritesheetPath.string())
{
    if (!renderer_)
        throw std::invalid_argument("RenderSystem: renderer cannot be null");
}

RenderComponent RenderSystem::resolveTile(int gridX, int gridY, const RenderComponent& tileRender) const
{
    SpriteRegion region = tileAtlas_.getTileSprite(tileRender.name);

    RenderComponent resolved = tileRender;
    resolved.texturePath = tileTexturePath_;
    resolved.textureRect = sf::IntRect(region.subrect);
    resolved.sourceTileSize = region.sourceTileSize;
    resolved.renderX = static_cast<float>(gridX) * GameConstants::TILE_SIZE;
    resolved.renderY = static_cast<float>(gridY) * GameConstants::TILE_SIZE;
    return resolved;
}

void RenderSystem::render(GameController &controller, const Camera &camera, float dt)
{
    renderer_->clear();
    renderer_->beginWorldView(camera);

    World *world = controller.getWorld();
    Entity *player = controller.getPlayer();

    if (!world || !player)
    {
        renderer_->present();
        return;
    }

    const Map *map = world->getActiveMap();

    if (map)
    {
        for (int y = 0; y < map->getHeight(); ++y)
        {
            for (int x = 0; x < map->getWidth(); ++x)
            {
                RenderComponent tile = resolveTile(x, y, map->tile_at(x, y).getRenderComponent());
                renderer_->drawEntity(tile, RenderAnchor::TopLeft);
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
                [this, mapObjRender = *mapObjRender]()
                {
                    renderer_->drawEntity(mapObjRender, RenderAnchor::CenterBottom);
                }});
        }

        // ---- Player -----------------------------------------------------

        const auto *playerPos = player->get<PositionComponent>();
        const auto *playerCollision = player->get<CollisionComponent>();
        const auto *playerRender = player->get<RenderComponent>();

        if (playerPos && playerCollision && playerRender)
        {
            AABB playerBox = playerCollision->resolve(playerPos->x, playerPos->y);
            renderer_->drawDebugRect(playerBox.x, playerBox.y, playerBox.width, playerBox.height);

            RenderComponent resolvedPlayer = *playerRender;
            resolvedPlayer.renderX = playerPos->x;
            resolvedPlayer.renderY = playerPos->y;

            renderables.push_back(Renderable{
                playerBox.y + playerBox.height,
                [this, resolvedPlayer]()
                {
                    renderer_->drawEntity(resolvedPlayer, RenderAnchor::CenterBottom);
                }});
        }

        std::sort(renderables.begin(), renderables.end(),
                  [](const Renderable &a, const Renderable &b)
                  {
                      return a.depthY < b.depthY;
                  });

        for (auto &r : renderables)
            r.draw();
    }
    else
    {
        const auto *playerPos = player->get<PositionComponent>();
        const auto *playerMove = player->get<FreeMovementComponent>();
        const auto *playerRender = player->get<RenderComponent>();
        if (playerPos && playerMove && playerRender)
        {
            RenderComponent resolvedPlayer = *playerRender;
            resolvedPlayer.renderX = playerPos->x;
            resolvedPlayer.renderY = playerPos->y;
            renderer_->drawEntity(resolvedPlayer, RenderAnchor::CenterBottom);
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