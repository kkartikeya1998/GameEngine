#pragma once
#include <variant>
#include <SFML/System/Vector2.hpp>
#include "entities/EntityID.h"

struct PlayerJumped { sf::Vector2f position; };
struct InventoryOpened {};
struct GamePaused {};
struct GameResumed {};
struct ItemConsumed{};
struct InteractionRequested { EntityID actor; EntityID target; std::string interactionId; };
struct DialogueFinished {};
struct BattleFinished {};

using GameEvent = std::variant<
    PlayerJumped,
    InventoryOpened,
    GamePaused,
    GameResumed,
    ItemConsumed,
    InteractionRequested,
    DialogueFinished,
    BattleFinished
>;