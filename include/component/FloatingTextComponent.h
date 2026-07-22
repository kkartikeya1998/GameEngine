#pragma once
#include <SFML/System/Vector2.hpp>
#include "entities/Component.h"
#include "entities/EntityID.h"

// Attaches a presentation-only entity (e.g. a "+10 HP" popup) to an owner
// entity so it can track the owner's position and animate independently
// of it. totalSeconds mirrors whatever duration the entity's
// LifetimeComponent was created with — kept here rather than added to
// LifetimeComponent itself so LifetimeComponent stays a generic countdown
// with no knowledge of fading.
struct FloatingTextComponent : public Component {
    EntityID owner;
    sf::Vector2f baseOffset;   // offset from owner's position at spawn time
    float riseSpeed = 20.f;    // pixels/second drifted upward over totalSeconds
    float totalSeconds = 1.f;  // must match the LifetimeComponent this entity was given

    FloatingTextComponent() = default;
    FloatingTextComponent(EntityID owner, sf::Vector2f baseOffset, float riseSpeed, float totalSeconds)
        : owner(owner), baseOffset(baseOffset), riseSpeed(riseSpeed), totalSeconds(totalSeconds) {}
};