// DialogueBox.h
#pragma once

#include "ui/UIComponent.h"
#include "ui/UIType.h"
#include "render/RenderLayer.h"
#include "render/RenderSystem.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

class DialogueBox : public UIComponent {
public:
    DialogueBox(const sf::Font& font,
                std::string text,
                float x, float y,
                float width, float height)
        : UIComponent(UIType::NonDiegetic)
        , font_(font)
        , textString_(std::move(text))
        , x_(x)
        , y_(y)
        , width_(width)
        , height_(height)
    {}

    void setText(std::string text) {
        textString_ = std::move(text);
    }

    void render(RenderSystem& renderSystem) override {
        // Background panel – screen space.
        // Choose a UI layer above world, and a fixed z.
        constexpr RenderLayer kUiLayer = RenderLayer::UiOverlay; // or whatever you use
        constexpr float       kUiZ     = 0.f;

        renderSystem.submitRect(
            kUiLayer,
            kUiZ,
            x_, y_,
            width_, height_,
            sf::Color(0, 0, 0, 180), // semi‑transparent black
            /*screenSpace=*/true
        );

        // Text – screen space. We'll create an sf::Text and let IRenderer draw it.
        submitText(renderSystem);
    }

private:
    const sf::Font& font_;
    std::string     textString_;
    float           x_;
    float           y_;
    float           width_;
    float           height_;

    void submitText(RenderSystem& renderSystem);
};