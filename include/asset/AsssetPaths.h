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

    inline const std::filesystem::path CHARACTERS =
        ROOT / "characters";

    inline const std::filesystem::path FONTS =
        ROOT / "fonts";

    namespace Maps
    {
        inline const std::filesystem::path TILESET_METADATA =
            MAPS / "tileset_metadata.json";

        inline const std::filesystem::path OBJECT_METADATA =
            MAPS / "object_metadata.json";
    }

    namespace Characters
    {
        inline const std::filesystem::path CHARACTERS_METADATA =
            CHARACTERS / "characters_metadata.json";

        inline const std::filesystem::path PLAYER_SPRITESHEET =
            CHARACTERS / "player.png";
    }

    namespace Objects
    {
        inline const std::filesystem::path SIMPLE_SUMMER_TILES =
            OBJECTS / "SimpleSummer_Tiles.png";

        inline const std::filesystem::path SIMPLE_SUMMER_OBJECTS =
            OBJECTS / "SimpleSummer_Objects.png";
    }

    namespace Fonts
    {
        inline const std::filesystem::path PIXFAY =
            FONTS / "Pixfay.otf";
    }
}