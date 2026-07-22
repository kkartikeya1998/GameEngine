#include "state/DialogueState.h"
#include "input/InputManager.h"
#include "asset/AsssetPaths.h"
#include "ui/UISystem.h"
#include "state/MenuInput.h"
#include "log/Logger.h"
#include "events/EventQueue.h"   // needed: services_.events.Push() requires complete EventQueue type
#include "events/Events.h"       // needed: DialogueFinished lives here

void DialogueState::OnEnter()
{
    LOG_INFO("Entering state: " + text_);
}

void DialogueState::OnExit()
{
    LOG_INFO("Exiting state");
}

DialogueState::DialogueState(GameServices services,
                             std::string text, std::filesystem::path fontPath)
    : services_(services), text_(std::move(text))
{
    LOG_INFO("Creating state");
    fontPath_ = fontPath.empty() ? std::filesystem::path(Assets::Fonts::PIXFAY) : std::move(fontPath);

    // Advance/dismiss on Enter or E; Escape also dismisses — same "back = pop" intent as menus
    navInput_.bind(Key::Enter, std::make_shared<ConfirmCommand>(), TriggerMode::Press);
    navInput_.bind(Key::E, std::make_shared<ConfirmCommand>(), TriggerMode::Press);
    MenuInput::BindBackKey(navInput_, Key::Escape);

    box_.text = text_;
    box_.x = 100.f;
    box_.y = 400.f;
    box_.width = 600.f;
    box_.height = 150.f;
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