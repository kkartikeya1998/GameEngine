#include "game/state/PauseState.h"
#include "engine/logging/Logger.h"
#include "game/state/StateMachine.h"
#include "engine/input/InputManager.h"
#include "engine/assets/AssetPaths.h"
#include "game/ui/UISystem.h"
#include "game/state/MenuInput.h"

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

PauseState::PauseState(GameServices services,
                       std::filesystem::path fontPath)
    : services_(services)
{
    LOG_INFO("Creating state");
    fontPath_ = fontPath.empty() ? std::filesystem::path(Assets::Fonts::PIXFAY) : std::move(fontPath);

    MenuInput::BindDefaults(navInput_);
    MenuInput::BindBackKey(navInput_, Key::Escape);

    const float boxWidth = 600.f;
    const float boxHeight = 120.f;

    panel_.type = UIType::NonDiegetic; // pause menu is pure UI, not in-world
    panel_.layout = {
        HorizontalAnchor::Center,
        VerticalAnchor::Top, // Top + offset reproduces the old *0.7f ratio exactly
        {.mode = UISizeMode::Fixed, .value = {boxWidth, boxHeight}},
        {.x = 0.f, .y = (GameConstants::GAME_RESOLUTION_H - boxHeight) * 0.7f}};
    panel_.options.push_back({"Continue", std::make_shared<ResumeCommand>()}); // index 0, default selection
    panel_.options.push_back({"Quit", std::make_shared<QuitCommand>()});       // index 1
}

void PauseState::Update(float dt)
{
    LOG_INFO("Updating state");

    MenuContext nav = navInput_.poll(services_.input); // translate raw keys to menu intent

    if (UISystem::HandleDefaultBack(nav, services_.states))
        return; // Escape already popped — don't also run navigation this frame

    PauseActionContext actionCtx{services_.states, services_.input}; // what Resume/Quit commands need
    UISystem::HandleNavigation(panel_, nav, actionCtx);              // move cursor / fire selected command
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