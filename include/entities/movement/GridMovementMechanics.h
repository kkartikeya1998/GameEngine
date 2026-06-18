#pragma once

#include "entities/movement/IMovementMechanics.h"

class GridMovementMechanics : public IMovementMechanics {
public:
    GridMovementMechanics(int x, int y, Direction dir = Direction::NONE);

    // -------------------------
    // Position interface
    // -------------------------
    Position getPosition() const override;

    void setPosition(int x, int y) override;
    void setDirection(Direction dir) override;
    Direction getDirection() const override;

    // -------------------------
    // Movement interface
    // -------------------------
    void move(Direction dir) override;
    Position nextPos(Direction dir) const override;

    // -------------------------
    // Modifier system (buffs/debuffs)
    // // -------------------------
    // void addModifier(std::unique_ptr<IMovementModifier> modifier);
    // void clearModifiers();

private:
    struct MoveResult {
        int dx = 0;
        int dy = 0;
        int speed = 1;
    };

    // MoveResult compute(Direction dir) const;

    Position position_;
    // std::vector<std::unique_ptr<IMovementModifier>> modifiers_;
};