#pragma once
#include <variant>
#include <SFML/System/Vector2.hpp>

struct PlayerJumped { sf::Vector2f position; };
struct InventoryOpened {};
struct GamePaused {};
struct GameResumed {};
struct ItemConsumed{};
// ... grows as real needs appear (second-pressure applies here too —
// don't pre-populate events nothing publishes yet)

using GameEvent = std::variant<
    PlayerJumped,
    InventoryOpened,
    GamePaused,
    GameResumed,
    ItemConsumed
>;