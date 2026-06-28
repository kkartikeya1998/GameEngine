#include "render/RenderSystem.h"
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

void RenderSystem::render(GameController& controller)
{
    renderer_->clear();

    World* world = controller.getWorld();
    auto* player = controller.getPlayer();

    std::cout << "Player" << player->getRenderX() << "\n";

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

        for (const auto& obj : map->getMapObjects()) {
            const auto* raw = obj.get();
            std::optional<AABB> collisionBox = raw->getCollisionBox();

            if (collisionBox) {
                renderer_->drawDebugRect(collisionBox->x, collisionBox->y, collisionBox->width, collisionBox->height);
            }

            float depthY = collisionBox
                ? (collisionBox->y + collisionBox->height)
                : static_cast<float>(raw->getOriginPixelY());

            float originX = static_cast<float>(raw->getOriginPixelX());
            float originY = static_cast<float>(raw->getOriginPixelY());
            std::string typeName = raw->getTypeName();

            renderables.push_back(Renderable{
                depthY,
                [this, originX, originY, typeName = std::move(typeName)]() {
                    renderer_->drawMapObject(originX, originY, typeName);
                }
            });
        }

        // for (const auto& npc : map->getNpcs()) {
        //     Entity* raw = npc.get();

        //     float renderX = raw->getRenderX();
        //     float renderY = raw->getRenderY();
        //     Direction facing = raw->getDirection();
        //     float animProgress = raw->getAnimProgress();

        //     renderables.push_back(Renderable{
        //         renderY,
        //         [this, renderX, renderY, facing, animProgress]() {
        //             renderer_->drawPlayer(renderX, renderY, facing, animProgress);
        //         }
        //     });
        // }

        AABB playerBox = player->getHitbox();
        renderer_->drawDebugRect(playerBox.x, playerBox.y, playerBox.width, playerBox.height);

        {
            float renderX = player->getRenderX();
            float renderY = player->getRenderY();
            Direction facing = player->getDirection();
            float animProgress = player->getAnimProgress();

            renderables.push_back(Renderable{
                playerBox.y + playerBox.height,
                [this, renderX, renderY, facing, animProgress]() {
                    renderer_->drawPlayer(renderX, renderY, facing, animProgress);
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
        renderer_->drawPlayer(
            player->getRenderX(),
            player->getRenderY(),
            player->getDirection(),
            player->getAnimProgress()
        );
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