#pragma once

#include <memory>
#include <string>
#include "engine/ecs/Registry.h"
#include "engine/services/GameServices.h"
#include "game/state/IGameState.h"


class InteractionManager;   // forward declaration — reference-only usage
// ---------------------------------------------------------------------------
// StateFactory — centralizes construction of pushable states so callers
// (GameplayState, InteractionManager, ...) depend on this interface instead
// of including every concrete state header directly.
// ---------------------------------------------------------------------------
namespace StateFactory
{
    std::unique_ptr<IGameState> MakeGameplay(GameServices services, InteractionManager &interactions);
    std::unique_ptr<IGameState> MakePause(GameServices services);
    std::unique_ptr<IGameState> MakeInventory(GameServices services, Registry &registry, EntityID player);
    std::unique_ptr<IGameState> MakeDialogue(GameServices services, std::string text);
}