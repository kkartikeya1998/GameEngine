#pragma once

#include <SFML/Graphics.hpp>
#include "external/json.hpp"
#include "asset/metadata/CollisionBox.h"
#include "asset/metadata/RenderBox.h"

namespace MetadataParser {
    // Convenience functions for parsing complete component metadata from JSON objects

inline CollisionBox parse_collision_box(
    const nlohmann::json& collision)
{
    CollisionBox box;

    box.offsetX = collision.value("offset_x", 0.f);
    box.offsetY = collision.value("offset_y", 0.f);
    box.width   = collision.value("width", 0.f);
    box.height  = collision.value("height", 0.f);

    return box;
}
}