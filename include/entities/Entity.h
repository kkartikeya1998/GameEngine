#pragma once

#include <memory>

#include "entities/movement/IMovementMechanics.h"
#include "animation/AnimationComponent.h"

class Entity {
public:
    explicit Entity(std::unique_ptr<IMovementMechanics> movement);
    virtual ~Entity() = default;

    // movement request
    virtual void move(Direction dir);

    // update every frame
    void update(float dt);

    // logical position
    int getX() const;
    int getY() const;

    Direction getDirection() const;

    // render position (smooth)
    float getRenderX() const;
    float getRenderY() const;

    // movement mechanics access
    IMovementMechanics& movement();
    const IMovementMechanics& movement() const;

    // animation state
    bool isAnimating() const;
    float getAnimationProgress() const;
protected:
    std::unique_ptr<IMovementMechanics> movement_;
    AnimationComponent animation_;
};