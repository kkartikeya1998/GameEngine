#include "state/StateFactory.h"
#include "state/PauseState.h"
#include "state/InventoryState.h"
#include "state/DialogueState.h"
#include "state/GameplayState.h"

namespace StateFactory
{
    std::unique_ptr<IGameState> MakeGameplay(GameServices services)
    {
        return std::make_unique<GameplayState>(services);
    }

    std::unique_ptr<IGameState> MakePause(GameServices services)
    {
        return std::make_unique<PauseState>(services);
    }

    std::unique_ptr<IGameState> MakeInventory(GameServices services, Registry &registry, EntityID player)
    {
        return std::make_unique<InventoryState>(services, registry, player);
    }

    std::unique_ptr<IGameState> MakeDialogue(GameServices services, std::string text)
    {
        return std::make_unique<DialogueState>(services, std::move(text));
    }
}