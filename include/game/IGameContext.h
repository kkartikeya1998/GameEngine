#pragma once

class Registry;


// NEED TO REMOVE SOMEHOW. Exists so the services dont know about the game itself.
class IGameContext
{
public:
    virtual ~IGameContext() = default;

    virtual Registry* GetRegistry() const = 0;
};