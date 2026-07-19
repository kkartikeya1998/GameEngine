#pragma once
#include <SFML/Graphics/Font.hpp>
#include <unordered_map>
#include "log/Logger.h"
#include <filesystem>
// Lazily loads and caches fonts by path

namespace UIFont
{
    inline const sf::Font &GetShared(const std::filesystem::path &path)
    {
        static std::unordered_map<std::filesystem::path, sf::Font> cache;

        if (auto it = cache.find(path); it != cache.end())
            return it->second;

        auto [it, inserted] = cache.emplace(path, sf::Font{});
        if (!it->second.openFromFile(path))
            LOG_WARNING("Failed to load font: " + path.string());

        return it->second;
    }
}