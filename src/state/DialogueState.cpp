#include "state/DialogueState.h"
#include "input/InputManager.h"
#include "state/MenuInput.h"
#include "asset/AsssetPaths.h"

sf::Font DialogueState::s_font_;

DialogueState::DialogueState(InputManager &input, StateMachine<IGameState> &stateMachine, EventQueue &events, std::string text)
    : input_(input), stateMachine_(stateMachine), events_(events), text_(std::move(text))
{
    static bool s_loaded = false;
    if (!s_loaded)
    {
        if (!s_font_.openFromFile(Assets::Fonts::PIXFAY))
            std::cerr << "[InventoryState] Failed to load font\n";
        else
            s_loaded = true;
    }
    MenuInput::BindDefaults(navInput_);
    box_ = std::make_unique<DialogueBox>(s_font_, text_, 50.f, 400.f, 700.f, 120.f);
}

void DialogueState::Update(float dt)
{
    MenuContext nav = navInput_.poll(input_);
    if (nav.confirm)
    {
        stateMachine_.Pop();
        events_.Push(DialogueFinished{});
    }
}

void DialogueState::Render(RenderSystem &renderSystem, float dt)
{
    box_->render(renderSystem);
}