#pragma once

#include <string>

#include "entities/Entity.h"

// ---------------------------------------------------------------------------
// Npc — MINIMAL PLACEHOLDER
// ---------------------------------------------------------------------------
class Npc {
public:
    Npc(Entity entity, std::string typeName)
        : entity_(std::move(entity))
        , typeName_(std::move(typeName))
    {
    }

    void update(float dt)
    {
        entity_.update(dt, Direction::NONE, [](const AABB&) { return false; });
    }

    Direction getDirection() const { return entity_.getDirection(); }
    float getRenderX() const { return entity_.getRenderX(); }
    float getRenderY() const { return entity_.getRenderY(); }

    const std::string& getTypeName() const { return typeName_; }

    Entity& entity() { return entity_; }
    const Entity& entity() const { return entity_; }

private:
    Entity entity_;
    std::string typeName_;
};