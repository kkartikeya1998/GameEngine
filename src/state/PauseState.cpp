#include <iostream>

#include "state/PauseState.h"
#include "state/StateMachine.h"
#include "input/InputManager.h"
#include "asset/AsssetPaths.h"
#include "ui/UISystem.h"
#include "state/MenuInput.h"

sf::Font PauseState::s_font_;

// Resumes gameplay by popping this state — OnResume() fires on GameplayState below
class ResumeCommand : public ICommand<PauseActionContext>
{
public:
    void execute(PauseActionContext &ctx) const override
    {
        ctx.stateMachine.Pop();
    }
};

// Requests application quit via InputManager
class QuitCommand : public ICommand<PauseActionContext>
{
public:
    void execute(PauseActionContext &ctx) const override
    {
        ctx.input.RequestQuit();
    }
};

PauseState::PauseState(InputManager &input,
                       StateMachine<IGameState> &stateMachine)
    : input_(input), stateMachine_(stateMachine), font_(s_font_)
{
    // std::cout << "[PauseState] Created\n";

    // Lazy initialization: load once on first construction
    static bool s_loaded = false;
    if (!s_loaded)
    {
        if (!s_font_.openFromFile(Assets::Fonts::PIXFAY))
        {
            std::cerr << "[PauseState] Failed to load font: "
                      << Assets::Fonts::PIXFAY << "\n";
        }
        else
        {
            s_loaded = true;
            // std::cout << "[PauseState] Font loaded: "
            //           << Assets::Fonts::PIXFAY << "\n";
        }
    }

    MenuInput::BindDefaults(navInput_); // Up/Down/Enter
    MenuInput::BindBackKey(navInput_, Key::Escape);

    const float boxWidth = 600.f;
    const float boxHeight = 120.f;

    panel_.type = UIType::NonDiegetic; // pause menu is pure UI, not in-world
    panel_.x = (GameConstants::GAME_RESOLUTION_W - boxWidth) * 0.5f;
    panel_.y = (GameConstants::GAME_RESOLUTION_H - boxHeight) * 0.7f;
    panel_.width = boxWidth;
    panel_.height = boxHeight;
    panel_.options.push_back({"Continue", std::make_shared<ResumeCommand>()}); // index 0, default selection
    panel_.options.push_back({"Quit", std::make_shared<QuitCommand>()});       // index 1
}

void PauseState::Update(float dt)
{
    // if (input_.WasKeyPressed(Key::Escape))
    // {
    //     input_.RequestQuit(); // direct quit shortcut, kept separate from panel cancel semantics — revisit once a submenu needs "back" instead of "quit"
    // }

    MenuContext nav = navInput_.poll(input_); // translate raw keys to menu intent

    if (UISystem::HandleDefaultBack(nav, stateMachine_))
        return; // Escape already popped — don't also run navigation this frame

    PauseActionContext actionCtx{stateMachine_, input_}; // what Resume/Quit commands need
    UISystem::HandleNavigation(panel_, nav, actionCtx);  // move cursor / fire selected command
}

void PauseState::Render(RenderSystem &renderSystem, float dt)
{
    renderSystem.submitRect(
        RenderLayer::ScreenOverlay, 0.f,
        0.f, 0.f,
        GameConstants::GAME_RESOLUTION_W,
        GameConstants::GAME_RESOLUTION_H,
        sf::Color(0, 0, 0, 150),
        /*screenSpace=*/true); // fullscreen dim, drawn beneath the panel

    UISystem::Render(panel_, renderSystem, font_); // panel backdrop + options, replaces old hardcoded box/text
}