// #pragma once

// #include <SFML/Graphics.hpp>
// #include "external/json.hpp"
// #include "asset/metadata/CollisionData.h"

// namespace MetadataParser {
//     // Convenience functions for parsing complete component metadata from JSON objects

// inline CollisionData parse_collision_box(
//     const nlohmann::json& collision)
// {
//     CollisionData box;

//     box.offsetX = collision.value("offset_x", 0.f);
//     box.offsetY = collision.value("offset_y", 0.f);
//     box.width   = collision.value("width", 0.f);
//     box.height  = collision.value("height", 0.f);
//     box.solid = collision.value("solid", false);

//     return box;
// }
// }