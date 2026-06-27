#include "render/RenderSystem.h"
#include <algorithm>
#include <vector>
#include <functional>

namespace {
    // NEW — depth-sortable unit of work. depthY is the bottom-edge pixel
    // Y (feet/AABB max-Y), matching the anchor convention drawMapObject
    // and drawPlayer already use (both anchor by bottom-center). Sorting
    // ascending by depthY and drawing in that order means whichever
    // thing's feet are lower on screen draws on top — correct overlap
    // for a top-down 2D scene without any per-category draw passes.
    struct Renderable {
        float depthY;
        std::function<void()> draw;
    };
}

RenderSystem::RenderSystem(std::unique_ptr<IRenderer> renderer)
    : renderer_(std::move(renderer)) {
    if (!renderer_) {
        throw std::invalid_argument("RenderSystem: renderer cannot be null");
    }
}

void RenderSystem::render(GameController& controller) {
    // Clear screen for new frame
    renderer_->clear();

    // Get references to world and player
    World* world = controller.getWorld();
    FreeEntity* player = controller.getPlayer();

    if (!world || !player) {
        renderer_->present();
        return;
    }

    // -------------------------
    // Draw map (tiles) — unchanged: tiles tile a full grid with no
    // transparency gaps and never meaningfully overlap something at a
    // different depth, so they stay a separate first pass (floor layer).
    // -------------------------
    Map* map = world->getActiveMap();

    if (map) {
        for (int y = 0; y < map->getHeight(); y++) {
            for (int x = 0; x < map->getWidth(); x++) {
                renderer_->drawTile(
                    x,
                    y,
                    map->tile_at(x, y).typeName()
                );
            }
        }

        // -------------------------
        // CHANGED: map objects, NPCs, and the player no longer draw in
        // three separate hardcoded passes (which ignored Y-position
        // across categories — a tree behind the player would still draw
        // after and cover them). All three now feed one Renderable
        // queue, sorted by depthY once, then drawn in that order.
        // -------------------------
        std::vector<Renderable> renderables;
        renderables.reserve(map->getMapObjects().size() + map->getNpcs().size() + 1);

        for (const auto& obj : map->getMapObjects()) {
            if (auto box = obj->getCollisionBox(GameConstants::TILE_SIZE)) {
                renderer_->drawDebugRect(box->x, box->y, box->width, box->height);
            }
            renderables.push_back({
                static_cast<float>(obj->getOriginPixelY()),
                [this, &obj]() {
                    renderer_->drawMapObject(
                        obj->getOriginPixelX(),
                        obj->getOriginPixelY(),
                        obj->getTypeName()
                    );
                }
            });
        }
        

        // NEW — NPCs feed the same queue as objects/player. Npc exposes
        // the same getRenderX/Y()/getDirection() surface Player already
        // does (forwarded to whichever Entity it currently owns), so no
        // NPC-vs-player branching is needed here.
        for (const auto& npc : map->getNpcs()) {
            renderables.push_back({
                npc->getRenderY(),
                [this, &npc]() {
                    // PLACEHOLDER: reuses drawPlayer's signature since Npc
                    // has no sprite-region lookup of its own yet (no NPC
                    // atlas/typeName-based draw call exists). Once NPC
                    // sprite assets exist, this likely becomes a real
                    // renderer_->drawNpc(originX, originY, typeName, ...)
                    // — a new IRenderer method (additive), not a change
                    // to this loop's shape or the sort itself.
                    renderer_->drawPlayer(
                        npc->getRenderX(),
                        npc->getRenderY(),
                        npc->getDirection(),
                        0.0f // no walk cycle yet for static/placeholder NPCs
                    );
                }
            });
        }

    AABB playerBox = player->getHitbox();
    renderer_->drawDebugRect(playerBox.x, playerBox.y, playerBox.width, playerBox.height);
        renderables.push_back({
            player->getRenderY(),
            [this, player]() {
                renderer_->drawPlayer(
                    player->getRenderX(),
                    player->getRenderY(),
                    player->getDirection(),
                    player->getWalkProgress()
                );
            }
        });

        std::sort(renderables.begin(), renderables.end(),
                  [](const Renderable& a, const Renderable& b) {
                      return a.depthY < b.depthY;
                  });

        for (auto& r : renderables) {
            r.draw();
        }
    } else {
        // No active map at all — still draw the player so the game
        // doesn't render a totally blank frame.
        renderer_->drawPlayer(
            player->getRenderX(),
            player->getRenderY(),
            player->getDirection(),
            player->getWalkProgress()
        );
    }

    // Present the frame
    renderer_->present();
}

bool RenderSystem::isOpen() const {
    return renderer_->isOpen();
}
std::optional<sf::Event> RenderSystem::pollEvent(){
    return renderer_->pollEvent();
}