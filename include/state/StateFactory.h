#pragma once

#include <memory>
#include <string>
#include "entities/Registry.h"
#include "game/GameServices.h"
#include "state/IGameState.h"

// ---------------------------------------------------------------------------
// StateFactory — centralizes construction of pushable states so callers
// (GameplayState, InteractionManager, ...) depend on this interface instead
// of including every concrete state header directly.
// ---------------------------------------------------------------------------
namespace StateFactory
{
    std::unique_ptr<IGameState> MakeGameplay(GameServices services);
    std::unique_ptr<IGameState> MakePause(GameServices services);
    std::unique_ptr<IGameState> MakeInventory(GameServices services, Registry &registry, EntityID player);
    std::unique_ptr<IGameState> MakeDialogue(GameServices services, std::string text);
}