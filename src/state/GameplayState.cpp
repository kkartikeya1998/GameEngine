#include "input/InputManager.h"

#include "render/RenderSystem.h"

#include "system/CameraSystem.h"

#include "state/GameplayState.h"
#include "state/GameplayCommands.h"
#include "state/PauseState.h"

#include "component/PositionComponent.h"
#include "component/SpriteAssetComponent.h"
#include "component/SpriteFrameComponent.h"
#include "component/TextRenderComponent.h"
#include "component/CollisionComponent.h"
#include "system/MovementAnimationSystem.h"
#include "system/VelocityIntegrationSystem.h"
#include "log/Logger.h"

GameplayState::GameplayState(InputManager &input, AssetDatabase &assets, StateMachine<IGameState> &stateMachine,
                             AnimationSystem &animationSystem, EventQueue &events, std::filesystem::path fontPath)
    : input_(input), assets_(assets), stateMachine_(stateMachine), animationSystem_(animationSystem),
      events_(events), fontPath_(std::move(fontPath))
{
    LOG_INFO("Creating state");
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
    LOG_INFO("Entering State");
    // Map/player load happens on entering gameplay, not at app boot.
    controller_ = std::make_unique<GameController>(1, 600, 600, assets_, events_);

    if (!fontPath_.empty())
    {
        fontLoaded_ = font_.openFromFile(fontPath_); // SFML 3 API
        if (!fontLoaded_)
            LOG_WARNING(std::format("GameplayState: failed to load font at {}", fontPath_.string()));
    }

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
    LOG_INFO("Exiting state");
    // [TODO] Proper cleanup or save of world state, player state, etc.
    controller_.reset();
}

#include "state/InventoryState.h" // new include, alongside PauseState's

void GameplayState::Update(float dt)
{
    LOG_INFO("Updating State");
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
        return;
    }

    PlayerControlComponent input = bindings_.poll(input_);
    controller_->update(dt, input);


    auto &registry = controller_->getWorld()->registry();

    VelocityIntegrationSystem::update(registry, dt);
    MovementAnimationSystem::update(registry); // new 
    animationSystem_.update(registry, dt);     // moved from Render()

    if (auto *pos = registry.get<PositionComponent>(controller_->getPlayer()))
    {
        CameraSystem::update(*pos, controller_->getActiveMap(), camera_);
    }
}

void GameplayState::Render(RenderSystem &renderSystem, float dt)
{
    LOG_INFO("Rendering State");
    World *world = controller_->getWorld();
    if (!world)
        return;

    const Map &map = world->getActiveMap();
    Registry &registry = world->registry();

    for (int y = 0; y < map.getHeight(); ++y)
        for (int x = 0; x < map.getWidth(); ++x)
        {
            // Tile::getRenderComponent() → getSpriteAsset()/getSpriteFrame(); needs Tile.h.
            const auto &tile = map.tile_at(x, y);
            renderSystem.submitTile(x, y, tile.getSpriteAsset(), tile.getSpriteFrame());
        }

    for (EntityID id : registry.view<SpriteAssetComponent, SpriteFrameComponent, PositionComponent>())
    {
        const auto *asset = registry.get<SpriteAssetComponent>(id);
        const auto *frame = registry.get<SpriteFrameComponent>(id);
        const auto *pos = registry.get<PositionComponent>(id);
        const auto *collision = registry.get<CollisionComponent>(id);

        float z = static_cast<float>(pos->y);
        if (collision)
        {
            AABB box = collision->resolve(pos->x, pos->y);
            z = box.y + box.height;
            
#ifdef ENGINE_DEBUG
            renderSystem.submitDebugRect(box.x, box.y, box.width, box.height);
#endif
        }
        ResolvedSprite sprite{frame->texturePath, frame->textureRect, frame->sourceTileSize, asset->renderScale, pos->x, pos->y};
        renderSystem.submit(asset->layer, z, sprite, RenderAnchor::CenterBottom);
    }

    if (fontLoaded_)
        for (EntityID id : registry.view<TextRenderComponent, PositionComponent>())
        {
            const auto *text = registry.get<TextRenderComponent>(id);
            const auto *pos = registry.get<PositionComponent>(id);
            renderSystem.submitText(text->layer, pos->y, font_, text->text, pos->x, pos->y, text->characterSize, text->color, false);
        }
}