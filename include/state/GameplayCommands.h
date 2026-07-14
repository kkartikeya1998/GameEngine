#pragma once
#include "input/ICommand.h"
#include "component/PlayerControlComponent.h"

class MoveCommand : public ICommand<PlayerControlComponent> {
public:
    explicit MoveCommand(Direction dir) : direction_(dir) {}
    void execute(PlayerControlComponent& playerControl) const override { playerControl.direction = direction_; }
private:
    Direction direction_;
};

class SprintCommand : public ICommand<PlayerControlComponent> {
public:
    void execute(PlayerControlComponent& playerControl) const override { playerControl.sprinting = true; }
};

class JumpCommand : public ICommand<PlayerControlComponent> {
public:
    void execute(PlayerControlComponent& playerControl) const override { playerControl.jumpRequested = true; }
};