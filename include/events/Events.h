#pragma once
#include <variant>
#include <SFML/System/Vector2.hpp>
#include "entities/EntityID.h"

struct PlayerJumped { sf::Vector2f position; };
struct InventoryOpened {};
struct GamePaused {};
struct GameResumed {};
struct InteractionRequested { EntityID actor; EntityID target; std::string interactionId; };
struct DialogueFinished {};
struct BattleFinished {};
struct ItemConsumed { EntityID owner; std::string itemId; };
struct ItemPickedUp { EntityID owner; std::string itemId; int quantity; };

using GameEvent = std::variant<
    PlayerJumped,
    InventoryOpened,
    GamePaused,
    GameResumed,
    InteractionRequested,
    DialogueFinished,
    BattleFinished,
    ItemConsumed,
    ItemPickedUp
>;