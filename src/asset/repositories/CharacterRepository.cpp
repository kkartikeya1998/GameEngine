#include "asset/repositories/CharacterRepository.h"
#include "asset/metadata/MetadataParser.h"

#include <fstream>
#include <iostream>
#include "external/json.hpp"

using json = nlohmann::json;

CharacterRepository::CharacterRepository(const std::filesystem::path &metadataFilePath)
{
    load_from_file(metadataFilePath);
}

void CharacterRepository::load_from_file(const std::filesystem::path &path)
{
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open character metadata: " << path << '\n';
        return;
    }

    json j;
    file >> j;

    const auto &characters = j.at("characters");

    for (auto charIt = characters.begin(); charIt != characters.end(); ++charIt)
    {
        const std::string characterName = charIt.key();
        const json &character = charIt.value();

        std::string texturePath = character.value("texture", "");
        int tileSize = character.value("tile_size", GameConstants::TILE_SIZE);

        std::optional<CollisionBox> collision;
        if (character.contains("collision_component")) {
            collision = MetadataParser::parse_collision_box(character.at("collision_component"));
        }

        const auto &states = character.at("character_frames");

        for (auto stateIt = states.begin(); stateIt != states.end(); ++stateIt)
        {
            const std::string state = stateIt.key();
            const json &frames = stateIt.value();

            for (auto it = frames.begin(); it != frames.end(); ++it)
            {
                const std::string rawKey = it.key();
                const json &def = it.value();

                // Expect format: direction_index (e.g. "down_0")
                auto underscorePos = rawKey.find('_');
                std::string direction = rawKey.substr(0, underscorePos);
                std::string frameIndexStr = rawKey.substr(underscorePos + 1);
                int frameIndex = std::stoi(frameIndexStr);

                std::string finalName = characterName + "_" + state + "_" + direction + "_" + frameIndexStr;

                CharacterMetadata meta;
                meta.name = finalName;
                meta.characterName = characterName;
                meta.state = state;
                meta.direction = direction;
                meta.frameIndex = frameIndex;
                meta.collisionBox = collision;

                const auto& render = def.at("render_component");
                meta.renderBox.name = finalName;
                meta.renderBox.textureRect = sf::IntRect{
                    { render.value("x", 0), render.value("y", 0) },
                    { render.value("w", 0), render.value("h", 0) }
                };
                meta.renderBox.texturePath = texturePath;
                meta.renderBox.sourceTileSize = tileSize;

                std::string countKey = characterName + "_" + state + "_" + direction;
                auto& count = frameCounts_[countKey];
                count = std::max(count, frameIndex + 1);

                frames_.emplace(finalName, std::move(meta));
            }
        }
    }
}

const CharacterMetadata *CharacterRepository::find(const std::string &frameName) const
{
    auto it = frames_.find(frameName);
    return it == frames_.end() ? nullptr : &it->second;
}

int CharacterRepository::frameCount(const std::string& character, const std::string& state, const std::string& direction) const
{
    auto it = frameCounts_.find(character + "_" + state + "_" + direction);
    return it == frameCounts_.end() ? 0 : it->second;
}