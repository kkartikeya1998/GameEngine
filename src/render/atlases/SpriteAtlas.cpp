#include "render/atlases/SpriteAtlas.h"
#include <stdexcept>
#include <iostream>

SpriteAtlas::SpriteAtlas(const std::filesystem::path& spritesheet_path, const CharacterRepository& characterRepository)
    : Atlas(spritesheet_path)
    , characterRepository_(characterRepository)
{}

SpriteRegion SpriteAtlas::getCharacterSprite(const std::string& spriteName) const
{
    // std::cout << "[SpriteAtlas] getCharacterSprite: " << spriteName << std::endl;
    return regionFromRepository(characterRepository_, spriteName, spriteName);
    // std::cout << "[SpriteAtlas] getCharacterSprite: " << spriteName << "retrieved" << std::endl;
}