#include "engine/input/InputManager.h"

#include "engine/render/RenderSystem.h"

#include "game/gameplay/CameraSystem.h"

#include "game/state/GameplayState.h"
#include "game/state/GameplayCommands.h"
#include "game/state/StateFactory.h"

#include "game/ecs/movement/PositionComponent.h"
#include "game/ecs/graphics/SpriteAssetComponent.h"
#include "game/ecs/graphics/SpriteFrameComponent.h"
#include "game/ecs/graphics/TextRenderComponent.h"
#include "game/ecs/collision/CollisionComponent.h"
#include "game/ecs/animation/MovementAnimationSystem.h"
#include "game/ecs/movement/VelocityIntegrationSystem.h"
#include "engine/logging/Logger.h"

GameplayState::GameplayState(GameServices<GameEvent> services,
                             std::filesystem::path fontPath)
    : services_(services),
      interactions_(services_),
      animationSystem_(services.assets),
      fontPath_(std::move(fontPath))
{
    LOG_INFO("Creating state");

    auto up = std::make_shared<MoveCommand>(sf::Vector2f{0.f, -1.f});
    auto down = std::make_shared<MoveCommand>(sf::Vector2f{0.f, 1.f});
    auto left = std::make_shared<MoveCommand>(sf::Vector2f{-1.f, 0.f});
    auto right = std::make_shared<MoveCommand>(sf::Vector2f{1.f, 0.f});
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
    controller_ = std::make_unique<GameController>(
        1,   // map-id
        100, // spawning position
        100,
        services_.assets,
        services_.events);
    eventHandler_ = std::make_unique<GameplayEventHandler>(
        services_.events, services_.assets,
        controller_->getWorld()->registry(), interactions_);

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

    cameraEntity_ = registry.create();
    auto &cam = registry.add<CameraComponent>(cameraEntity_);
    cam.followTarget = controller_->getPlayer();

    if (auto *pos = registry.get<PositionComponent>(controller_->getPlayer()))
    {
        CameraSystem::update(*pos, controller_->getActiveMap(), cam);
    }
}

void GameplayState::OnExit()
{
    LOG_INFO("Exiting state");
    // [TODO] Proper cleanup or save of world state, player state, etc.
    eventHandler_.reset();
    controller_.reset();
}

void GameplayState::Update(float dt)
{
    LOG_INFO("Updating State");
    if (services_.input.WasKeyPressed(Key::Escape))
    {
        services_.states.Push(StateFactory::MakePause(services_));
        return;
    }
    if (services_.input.WasKeyPressed(Key::I))
    {
        services_.states.Push(StateFactory::MakeInventory(
            services_, controller_->getWorld()->registry(), controller_->getPlayer()));
        return;
    }

    PlayerControlComponent input = bindings_.poll(services_.input);
    controller_->update(dt, input);

    auto &registry = controller_->getWorld()->registry();

    VelocityIntegrationSystem::update(registry, dt);
    // MovementAnimationSystem::update(registry);
    animationSystem_.update(registry, dt);

    if (auto *pos = registry.get<PositionComponent>(controller_->getPlayer()))
    {
        if (auto *cam = registry.get<CameraComponent>(cameraEntity_))
            CameraSystem::update(*pos, controller_->getActiveMap(), *cam);
    }

    if (eventHandler_)
        eventHandler_->Process();

    interactions_.Update();
}

void GameplayState::Render(RenderSystem &renderSystem, float dt)
{
    World *world = controller_->getWorld();
    if (!world)
        return;

    Registry &registry = world->registry();

    if (const auto *cam = registry.get<CameraComponent>(cameraEntity_))
        renderSystem.SetCameraView(CameraView{cam->centerX, cam->centerY, cam->viewWidth, cam->viewHeight});

    const Map &map = world->getActiveMap();

    for (int y = 0; y < map.getHeight(); ++y)
    {
        for (int x = 0; x < map.getWidth(); ++x)
        {
            const auto &tile = map.tile_at(x, y);
            const auto &asset = tile.getSpriteAsset();
            const auto &frame = tile.getSpriteFrame();
            renderSystem.submitTile(x, y, TileDrawInfo{frame.name, asset.renderScale, asset.layer, asset.z});
        }
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