#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <array>
#include <SFML/Window/Event.hpp>

using Key = sf::Keyboard::Key;

struct Vec2 { float x = 0.f; float y = 0.f; };

// ---------------------------------------------------------------------------
// InputManager — answers raw physical-input questions only. It has no
// idea what "W" or "Escape" mean to the game; states decide that.
// ---------------------------------------------------------------------------
class InputManager {
public:
    void PollEvents();
    void ProcessEvent(const sf::Event& event);

    bool ShouldQuit() const{ return shouldQuit_; }

    bool IsKeyDown(Key key) const;
    bool WasKeyPressed(Key key) const;

    Vec2 MousePosition() const;

private:
    bool shouldQuit_ = false;

    static constexpr int kKeyCount = sf::Keyboard::KeyCount;
    
    std::array<bool, kKeyCount> currentKeys_{};
    std::array<bool, kKeyCount> previousKeys_{};
};