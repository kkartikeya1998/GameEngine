#pragma once

#include <memory>
#include <string>

#include "entities/Entity.h"

// ---------------------------------------------------------------------------
// Npc — owns an Entity polymorphically (composition), rather than being one.
//
// WHY COMPOSITION INSTEAD OF "class Npc : public GridEntity" (or FreeEntity):
// GridEntity and FreeEntity drive fundamentally different animation models
// (AnimationComponent lerp-to-target vs. WalkCycleTimer elapsed-time —
// see FreeEntity.h's class comment), so "which movement family" isn't a
// detail, it's effectively which concrete class the NPC currently is.
// If Npc inherited from one of them, it could never legitimately become
// the other at runtime — that would be a Liskov violation (lying about
// its own type). By holding a pointer-to-base instead, switching which
// concrete Entity is active is just reassigning a pointer — no lying,
// no rewrite of GridEntity/FreeEntity/Player required.
//
// TODAY: active_ is set once at construction and never reassigned.
// switchMovement() does not exist yet — deliberately. This class is
// scoped to "NPCs exist, hold a position via whichever movement family
// fits them individually, and render correctly," per the current pass.
//
// FUTURE SEAM (not implemented): a later pass can add
//     void switchMovement(std::unique_ptr<Entity> newMode);
// which would copy position/facing from active_ into newMode (or leave
// that to the caller) and reassign active_. That method, and whatever
// behavior_ /fight-mode trigger eventually calls it, are the ONLY things
// that should need to change later — Map, MapLoader, RenderSystem, and
// GameController only ever touch Npc through the same getRenderX/Y(),
// getDirection(), update() surface they already use for Player, and
// none of them need to know switching is even possible.
//
// typeName_ exists for the same reason MapObject::getTypeName() exists:
// Entity's contract is position + direction, not "what sprite am I" —
// that lookup key has to live somewhere, and Npc is the minimal class
// that holds what NPCs need beyond Entity's existing contract.
// ---------------------------------------------------------------------------
class Npc {
public:
    Npc(std::unique_ptr<Entity> initialMovement, std::string typeName)
        : active_(std::move(initialMovement))
        , typeName_(std::move(typeName))
    {
    }

    // Forwarded surface — identical shape to what RenderSystem/GameController
    // already call on Player via Entity's virtuals. Callers holding an
    // Npc& never need to know it's not itself an Entity.
    void update(float dt) { active_->update(dt); }

    Direction getDirection() const { return active_->getDirection(); }
    float getRenderX() const { return active_->getRenderX(); }
    float getRenderY() const { return active_->getRenderY(); }

    const std::string& getTypeName() const { return typeName_; }

    // Escape hatch for code that genuinely needs the concrete Entity
    // (e.g. a future collision query mirroring isPositionBlocked, if
    // NPCs ever need their own movement validated against the map).
    Entity& current() { return *active_; }
    const Entity& current() const { return *active_; }

private:
    std::unique_ptr<Entity> active_;
    std::string typeName_;

    // FUTURE: std::unique_ptr<INpcBehavior> behavior_;  // nullptr today.
    // Not added yet — no behavior system exists to implement it against.
};