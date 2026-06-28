#pragma once

#include <string>

#include "entities/Entity.h"

// ---------------------------------------------------------------------------
// Npc — MINIMAL PLACEHOLDER, land-now version.
//
// WHY THIS FILE HAD TO CHANGE EVEN THOUGH NPCs AREN'T BEING BUILT OUT
// YET: Map.h already unconditionally #includes "entities/npc/Npc.h" and
// stores std::vector<std::unique_ptr<Npc>> — that wiring predates this
// redesign. The OLD npc.h called active_->update(dt) with ONE argument;
// the new Entity::update() requires THREE (dt, inputDir, isBlocked), no
// defaults. So the old npc.h would fail to compile the instant Entity.h
// changes, regardless of whether Npc itself is "done" — this isn't
// optional scope creep, it's the minimum edit required to keep
// Map/MapLoader/GameController compiling at all.
//
// This version is deliberately NOT the fuller Npc design (no
// computeInputDir() seam, no behavior_ comment, no escape-hatch
// entity() accessor beyond what's needed) — keeping it minimal since
// you said NPCs are still a dummy with no real constructor. Replace
// this file with the fuller version (see Npc_full.h provided alongside
// this) whenever you actually start building NPCs out — that version
// adds the behavior-driver seam and is a pure addition over this one,
// not a rewrite.
//
// update() forwards Direction::NONE and an always-false isBlocked,
// since nothing currently drives NPC movement and Map::isAreaBlocked
// isn't reachable from here without a Map reference. This matches
// "dummy" exactly: an Npc placed on a map sits still and renders at its
// spawn position, nothing more.
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