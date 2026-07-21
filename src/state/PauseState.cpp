#include "state/PauseState.h"
#include "log/Logger.h"
#include "state/StateMachine.h"
#include "input/InputManager.h"
#include "asset/AsssetPaths.h"
#include "ui/UISystem.h"
#include "state/MenuInput.h"

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

void PauseState::OnEnter()
{
    LOG_INFO("Entering state");
}

void PauseState::OnExit()
{
    LOG_INFO("Exiting state");
}

PauseState::PauseState(InputManager &input,
                       StateMachine<IGameState> &stateMachine,
                       std::filesystem::path fontPath)
    : input_(input), stateMachine_(stateMachine)
{
    LOG_INFO("Creating state");
    fontPath_ = fontPath.empty() ? std::filesystem::path(Assets::Fonts::PIXFAY) : std::move(fontPath);

    MenuInput::BindDefaults(navInput_);
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
    LOG_INFO("Updating state");

    MenuContext nav = navInput_.poll(input_); // translate raw keys to menu intent

    if (UISystem::HandleDefaultBack(nav, stateMachine_))
        return; // Escape already popped — don't also run navigation this frame

    PauseActionContext actionCtx{stateMachine_, input_}; // what Resume/Quit commands need
    UISystem::HandleNavigation(panel_, nav, actionCtx);  // move cursor / fire selected command
}

void PauseState::Render(RenderSystem &renderSystem, float dt)
{
    LOG_INFO("Rendering state");
    renderSystem.submitRect(
        RenderLayer::ScreenOverlay, 0.f,
        0.f, 0.f,
        GameConstants::GAME_RESOLUTION_W,
        GameConstants::GAME_RESOLUTION_H,
        sf::Color(0, 0, 0, 150),
        /*screenSpace=*/true); // fullscreen dim, drawn beneath the panel

    UISystem::Render(panel_, renderSystem, UIFont::GetShared(fontPath_)); // panel backdrop + options, replaces old hardcoded box/text
}