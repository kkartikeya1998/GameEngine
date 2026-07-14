#pragma once

#include <string>
#include "component/RenderComponent.h"

// ---------------------------------------------------------------------------
// A simple non-entity class but with a RenderComponent, representing a single tile in the map grid.
class Tile {
public:
    Tile() = default;

    // copy operations
    Tile(const Tile&) = default;
    Tile& operator=(const Tile&) = default;

    // move operations
    Tile(Tile&&) = default;
    Tile& operator=(Tile&&) = default;

    RenderComponent& getRenderComponent() { return renderComponent; }
    const RenderComponent& getRenderComponent() const {return renderComponent; }
    
    void addRenderComponent(const RenderComponent& render) { renderComponent = render; }
private:
    RenderComponent renderComponent;
};