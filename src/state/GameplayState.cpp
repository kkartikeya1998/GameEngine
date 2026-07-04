#include "state/GameplayState.h"
#include "state/GameplayCommands.h"
#include "input/InputManager.h"
#include "render/RenderSystem.h"

GameplayState::GameplayState(InputManager& input, AssetManager& assets, const std::string& assetsRoot)
    : input_(input), assets_(assets), assetsRoot_(assetsRoot)
{
    auto up = std::make_shared<MoveCommand>(Direction::UP);
    auto down = std::make_shared<MoveCommand>(Direction::DOWN);
    auto left = std::make_shared<MoveCommand>(Direction::LEFT);
    auto right = std::make_shared<MoveCommand>(Direction::RIGHT);
    auto sprint = std::make_shared<SprintCommand>();
    auto jump = std::make_shared<JumpCommand>();

    bindings_.bind(Key::W, up);
    bindings_.bind(Key::Up, up);
    bindings_.bind(Key::S, down);
    bindings_.bind(Key::Down, down);
    bindings_.bind(Key::A, left);
    bindings_.bind(Key::Left, left);
    bindings_.bind(Key::D, right);
    bindings_.bind(Key::Right, right);
    bindings_.bind(Key::LShift, sprint);
    bindings_.bind(Key::Space, jump, TriggerMode::Press);
}

void GameplayState::OnEnter()
{
    // Map/player load happens on entering gameplay, not at app boot.
    controller_ = std::make_unique<GameController>(
        1, 600, 600, assetsRoot_,
        assets_.get<MapObjectRepository>(), assets_.get<TileRepository>());
}

void GameplayState::OnExit()
{
    controller_.reset();
}

void GameplayState::Update(float dt)
{
    PlayerControlComponent input = bindings_.poll(input_);
    controller_->updatePlayerMovement(dt, input);
    controller_->update(dt);

    // Same key, different meaning per state — Escape pauses here,
    // but would quit in a MenuState. Wire the actual push once
    // PauseState exists.
    if (input_.WasKeyPressed(Key::Escape)) {
        // states_.Push(std::make_unique<PauseState>(...));
    }
}

void GameplayState::Render(RenderSystem& renderSystem, float dt)
{
    renderSystem.render(*controller_, dt);
}