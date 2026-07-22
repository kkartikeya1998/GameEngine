#pragma once

class Registry;

class IGameContext
{
public:
    virtual ~IGameContext() = default;

    virtual Registry* GetRegistry() const = 0;
};