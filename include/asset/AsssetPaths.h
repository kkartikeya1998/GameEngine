#pragma once

#ifndef PROJECT_ROOT
#error PROJECT_ROOT not defined
#endif

#include <filesystem>

namespace Assets
{
    inline const std::filesystem::path ROOT =
        std::filesystem::path(PROJECT_ROOT) / "assets";

    inline const std::filesystem::path MAPS =
        ROOT / "maps";

    inline const std::filesystem::path OBJECTS =
        ROOT / "objects";

    inline const std::filesystem::path SPRITES =
        ROOT / "sprites";

    namespace Maps
    {
        inline const std::filesystem::path TILESET_METADATA =
            MAPS / "tileset_metadata.json";

        inline const std::filesystem::path OBJECT_METADATA =
            MAPS / "object_metadata.json";
    }

    namespace Sprites
    {
        inline const std::filesystem::path SPRITE_METADATA =
            SPRITES / "sprite_metadata.json";

        inline const std::filesystem::path PLAYER_SPRITESHEET =
            SPRITES / "player_spritesheet.png";
    }

    namespace Objects
    {
        inline const std::filesystem::path SIMPLE_SUMMER_TILES =
            OBJECTS / "SimpleSummer_Tiles.png";

        inline const std::filesystem::path SIMPLE_SUMMER_OBJECTS =
            OBJECTS / "SimpleSummer_Objects.png";
    }
}