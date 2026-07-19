#include "state/DialogueState.h"
#include "input/InputManager.h"
#include "asset/AsssetPaths.h"
#include "ui/UISystem.h"
#include "state/MenuInput.h"
#include "log/Logger.h"

void DialogueState::OnEnter()
{
    LOG_INFO("Entering DialogueState: " + text_);
}

void DialogueState::OnExit()
{
    LOG_INFO("Exiting DialogueState");
}

DialogueState::DialogueState(InputManager &input, StateMachine<IGameState> &stateMachine, EventQueue &events,
                             std::string text, std::filesystem::path fontPath)
    : input_(input), stateMachine_(stateMachine), events_(events), text_(std::move(text))
{
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
    MenuContext nav = navInput_.poll(input_);

    if (nav.confirm || nav.cancel)
    {
        events_.Push(DialogueFinished{}); // tell InteractionManager this interaction is over, so it can re-arm
        stateMachine_.Pop();              // single-line dialogue: any acknowledge dismisses it
        return;
    }
}

void DialogueState::Render(RenderSystem &renderSystem, float dt)
{
    UISystem::Render(box_, renderSystem, UIFont::GetShared(fontPath_));
}