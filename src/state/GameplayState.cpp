#include "state/GameplayState.h"
#include "state/GameplayCommands.h"
#include "input/InputManager.h"
#include "render/RenderSystem.h"
#include "state/PauseState.h"
#include "system/CameraSystem.h"
#include "component/PositionComponent.h"
#include "component/RenderComponent.h"
#include "component/CollisionComponent.h"
#include "log/Logger.h"

GameplayState::GameplayState(InputManager &input, AssetDatabase &assets, StateMachine<IGameState> &stateMachine, AnimationSystem &animationSystem, EventQueue &events)
    : input_(input), assets_(assets), stateMachine_(stateMachine), animationSystem_(animationSystem), events_(events)
{
    auto up = std::make_shared<MoveCommand>(Direction::UP);
    auto down = std::make_shared<MoveCommand>(Direction::DOWN);
    auto left = std::make_shared<MoveCommand>(Direction::LEFT);
    auto right = std::make_shared<MoveCommand>(Direction::RIGHT);
    auto sprint = std::make_shared<SprintCommand>();
    auto jump = std::make_shared<JumpCommand>();
    auto interact = std::make_shared<InteractCommand>();

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
    bindings_.bind(Key::E, interact, TriggerMode::Press);
}

void GameplayState::OnEnter()
{
    // Map/player load happens on entering gameplay, not at app boot.
    controller_ = std::make_unique<GameController>(1, 600, 600, assets_, events_);

    // Center the camera on the player immediately, so the first
    // rendered frame isn't a flash at world-origin before the first
    // Update() call runs.
    auto &registry = controller_->getWorld()->registry();
    if (auto *pos = registry.get<PositionComponent>(controller_->getPlayer()))
    {
        CameraSystem::update(*pos, controller_->getActiveMap(), camera_);
    }
}

void GameplayState::OnExit()
{
    // [TODO] Proper cleanup or save of world state, player state, etc.
    controller_.reset();
}

#include "state/InventoryState.h" // new include, alongside PauseState's

void GameplayState::Update(float dt)
{
    if (input_.WasKeyPressed(Key::Escape))
    {
        stateMachine_.Push(std::make_unique<PauseState>(input_, stateMachine_));
        return;
    }

    if (input_.WasKeyPressed(Key::I))
    {
        stateMachine_.Push(std::make_unique<InventoryState>(
            input_, stateMachine_, controller_->getWorld()->registry(), controller_->getPlayer(),
            events_, assets_));
        return; // don't also process movement the frame we open inventory
    }

    PlayerControlComponent input = bindings_.poll(input_);
    controller_->update(dt, input);

    auto &registry = controller_->getWorld()->registry();
    if (auto *pos = registry.get<PositionComponent>(controller_->getPlayer()))
    {
        CameraSystem::update(*pos, controller_->getActiveMap(), camera_);
    }
}

void GameplayState::Render(RenderSystem &renderSystem, float dt)
{
    animationSystem_.update(controller_->getWorld()->registry(), dt);

    World *world = controller_->getWorld();
    if (!world)
        return;

    const Map &map = world->getActiveMap();
    Registry &registry = world->registry();

    // ---- Terrain -----------------------------------------------------
    for (int y = 0; y < map.getHeight(); ++y)
    {
        for (int x = 0; x < map.getWidth(); ++x)
        {
            renderSystem.submitTile(x, y, map.tile_at(x, y).getRenderComponent());
        }
    }

    // ---- Everything else: player, map objects, npcs — one pass -------
    // Previously three separate loops (map objects / player / npcs) doing
    // near-identical work. Now that all three live in the same registry
    // there's nothing distinguishing them at draw time — a single
    // RenderComponent+PositionComponent query covers all of them, in
    // whatever order the registry happens to iterate. Depth ordering is
    // handled by RenderSystem's (layer, z) sort on submit, not by draw
    // order here, so iteration order doesn't matter.
    for (EntityID id : registry.view<RenderComponent, PositionComponent>())
    {
        // std::cout << "[GameplayState] Entity in (render, position) view:" << id.index << " " << id.generation << "\n";
        const auto *entRender = registry.get<RenderComponent>(id);
        const auto *entPos = registry.get<PositionComponent>(id);

        const auto *entCollision = registry.get<CollisionComponent>(id);
        float z = static_cast<float>(entPos->y);

#ifdef ENGINE_DEBUG
        if (entCollision)
        {
            AABB box = entCollision->resolve(entPos->x, entPos->y);
            z = box.y + box.height;
            renderSystem.submitDebugRect(box.x, box.y, box.width, box.height);
        }
#endif

        RenderComponent resolved = *entRender;
        resolved.renderX = entPos->x;
        resolved.renderY = entPos->y;

        // std::cout << "Submitting :" << resolved.name << "\n";
        renderSystem.submit(resolved.layer, z, resolved, RenderAnchor::CenterBottom);
    }
}