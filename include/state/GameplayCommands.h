#pragma once

#include <SFML/System/Vector2.hpp>
#include "component/PlayerControlComponent.h"
#include "input/ICommand.h"

class MoveCommand : public ICommand<PlayerControlComponent>
{
public:
    explicit MoveCommand(sf::Vector2f movement)
        : movement_(movement)
    {
    }

    void execute(PlayerControlComponent &playerControl) const override
    {
        playerControl.move += movement_;
    }

private:
    sf::Vector2f movement_;
};

class SprintCommand : public ICommand<PlayerControlComponent>
{
public:
    void execute(PlayerControlComponent &playerControl) const override
    {
        playerControl.sprinting = true;
    }
};

class JumpCommand : public ICommand<PlayerControlComponent>
{
public:
    void execute(PlayerControlComponent &playerControl) const override
    {
        playerControl.jumpRequested = true;
    }
};

class InteractCommand : public ICommand<PlayerControlComponent>
{
public:
    void execute(PlayerControlComponent &playerControl) const override
    {
        playerControl.interactRequested = true;
    }
};