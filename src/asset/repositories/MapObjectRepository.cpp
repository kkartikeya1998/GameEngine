#include "asset/repositories/MapObjectRepository.h"
#include "asset/metadata/MetadataParser.h"

#include <fstream>
#include <iostream>

#include "external/json.hpp"

using json = nlohmann::json;

MapObjectRepository::MapObjectRepository(
    const std::filesystem::path &metadataFilePath)
{
    load_from_file(metadataFilePath);
}

void MapObjectRepository::load_from_file(
    const std::filesystem::path &path)
{
    std::ifstream file(path);

    if (!file)
    {
        std::cerr << "Failed to open object metadata: "
                  << path << '\n';
        return;
    }

    json j;
    file >> j;

    const auto &object_types =
        j.at("object_types");

    // ------------------------------------------------------
    // NEW: derive object set name from file-level texture
    // or fallback to filename stem
    // ------------------------------------------------------
    std::string objectSetName =
        j.value("object_set", "");

    if (objectSetName.empty())
    {
        objectSetName =
            path.stem().string();
    }
    int tileSize =
        j.value("tile_size",
                GameConstants::TILE_SIZE);

    for (auto it = object_types.begin();
         it != object_types.end();
         ++it)
    {
        const std::string objectType = it.key();
        const json &def = it.value();

        ObjectTypeMetadata meta;

        meta.name =
            objectSetName + "_" + objectType;

        // render data
        const auto& render = def.at("render_component");

        meta.renderBox.textureRect = sf::IntRect{
            { render.value("x", 0), render.value("y", 0) },
            { render.value("w", 0), render.value("h", 0) }
        };
        meta.renderBox.texturePath =
            j.value("texture", "");
        meta.renderBox.sourceTileSize = tileSize;

        // optional collision
        if (def.contains("collision_component"))
        {
            meta.collisionBox =
                MetadataParser::parse_collision_box(
                    def.at("collision_component"));
        }

        types_.emplace(meta.name, std::move(meta));
    }
}


const ObjectTypeMetadata* MapObjectRepository::find(
    const std::string& type) const
{
    std::cout << "[MapObjectRepository]: Searching for object type: " << type << "\n";
    auto it = types_.find(type);
    std::cout << "[MapObjectRepository]: Search result: " << (it == types_.end() ? "not found" : "found") << "\n";
    return it == types_.end() ? nullptr : &it->second;
}