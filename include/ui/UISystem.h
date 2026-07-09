#pragma once
#include "ui/Panel.h"
#include "ui/MenuContext.h"
#include "render/RenderSystem.h"
#include <SFML/Graphics/Font.hpp>

// Stateless navigation + rendering for Panel — mirrors MovementSystem's free-function pattern
namespace UISystem
{
    // Moves selectedIndex and fires the selected option's command on confirm
    template <typename ActionContext>
    void HandleNavigation(Panel<ActionContext> &panel, const MenuContext &nav, ActionContext &actionCtx)
    {
        const int optionCount = static_cast<int>(panel.options.size());
        if (optionCount == 0)
            return; // nothing to navigate

        if (nav.navigateDown)
            panel.selectedIndex = (panel.selectedIndex + 1) % optionCount; // wrap to top

        if (nav.navigateUp)
            panel.selectedIndex = (panel.selectedIndex - 1 + optionCount) % optionCount; // wrap to bottom

        if (nav.confirm)
            panel.options[panel.selectedIndex].command->execute(actionCtx); // delegate to bound action
    }

    // Draws panel background and each option, highlighting the current selection
    template <typename ActionContext>
    void Render(const Panel<ActionContext> &panel, RenderSystem &renderSystem, const sf::Font &font)
    {
        renderSystem.submitRect(
            RenderLayer::ScreenOverlay, 1.f,
            panel.x, panel.y, panel.width, panel.height,
            sf::Color(0, 0, 0, 200),
            /*screenSpace=*/true); // panel backdrop, above whatever tint the caller drew first

        const float paddingX = 20.f;
        const float paddingY = 20.f;
        const float lineHeight = 28.f;

        if (!panel.title.empty())
        {
            renderSystem.submitText(
                RenderLayer::ScreenOverlay, 2.f, font, panel.title,
                panel.x + paddingX, panel.y + paddingY,
                24u, sf::Color::White, true); // title line
        }

        const float listStartY = panel.y + paddingY + (panel.title.empty() ? 0.f : lineHeight);

        for (int i = 0; i < static_cast<int>(panel.options.size()); ++i)
        {
            const bool selected = (i == panel.selectedIndex);
            const std::string prefix = selected ? "> " : "  ";                       // cursor marker
            const sf::Color color = selected ? sf::Color::Yellow : sf::Color::White; // highlight selection

            renderSystem.submitText(
                RenderLayer::ScreenOverlay, 2.f, font, prefix + panel.options[i].label,
                panel.x + paddingX, listStartY + i * lineHeight,
                20u, color, true); // one line per option
        }
    }

    // Default "back = pop this state" behavior. Returns true if it fired, so the
    // caller's Update() can early-return the same way the hand-written
    // `if (nav.cancel) { stateMachine.Pop(); return; }` blocks did. A state that
    // needs different back behavior (e.g. closing a submenu instead of popping
    // the whole state) just skips this and handles nav.cancel itself.
    template <typename State>
    bool HandleDefaultBack(const MenuContext &nav, StateMachine<State> &stateMachine)
    {
        if (nav.cancel)
        {
            stateMachine.Pop();
            return true;
        }
        return false;
    }
}