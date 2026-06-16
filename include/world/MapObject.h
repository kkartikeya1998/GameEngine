#pragma once

class MapObject {
public:
    virtual ~MapObject() = default;
    virtual void render() const = 0;
};