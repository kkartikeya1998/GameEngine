#include "game/state/DialogueState.h"
#include "engine/assets/AssetPaths.h"
#include "game/ui/UISystem.h"
#include "game/state/MenuInput.h"

void DialogueState::OnEnter()
{
    LOG_INFO("Entering state: " + text_);
}

void DialogueState::OnExit()
{
    LOG_INFO("Exiting state");
}

DialogueState::DialogueState(GameServices<GameEvent> services,
                             std::string text, std::filesystem::path fontPath)
    : services_(services), text_(std::move(text))
{
    LOG_INFO("Creating state");
    fontPath_ = fontPath.empty() ? std::filesystem::path(Assets::Fonts::PIXFAY) : std::move(fontPath);

    navInput_.bind(Key::Enter, std::make_shared<ConfirmCommand>(), TriggerMode::Press);
    navInput_.bind(Key::E, std::make_shared<ConfirmCommand>(), TriggerMode::Press);
    MenuInput::BindBackKey(navInput_, Key::Escape);

    box_.text = text_;
    box_.layout = {
        HorizontalAnchor::Center,
        VerticalAnchor::Bottom,
        {.mode = UISizeMode::Fixed, .value = {600.f, 150.f}},
        {.x = 0.f, .y = -20.f} // 20px margin from the bottom edge
    };
}

void DialogueState::Update(float dt)
{
    LOG_INFO("Updating state");
    MenuContext nav = navInput_.poll(services_.input);

    if (nav.confirm || nav.cancel)
    {
        services_.events.Push(DialogueFinished{}); // tell InteractionManager this interaction is over, so it can re-arm
        services_.states.Pop();                    // single-line dialogue: any acknowledge dismisses it
        return;
    }
}

void DialogueState::Render(RenderSystem &renderSystem, float dt)
{
    LOG_INFO("Rendering state");
    UISystem::Render(box_, renderSystem, UIFont::GetShared(fontPath_));
}