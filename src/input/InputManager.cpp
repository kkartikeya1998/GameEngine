#include "input/InputManager.h"

void InputManager::PollEvents()
{
    previousKeys_ = currentKeys_;
    for (int i = 0; i < kKeyCount; ++i)
        currentKeys_[i] = sf::Keyboard::isKeyPressed(static_cast<Key>(i));
}

bool InputManager::IsKeyDown(Key key) const
{
    return currentKeys_[static_cast<int>(key)];
}

bool InputManager::WasKeyPressed(Key key) const
{
    int i = static_cast<int>(key);
    return currentKeys_[i] && !previousKeys_[i];
}

Vec2 InputManager::MousePosition() const
{
    // Desktop-space position — mapping into window/view space needs a
    // window reference I don't have without SFMLRenderer.h.
    auto pos = sf::Mouse::getPosition();
    return { static_cast<float>(pos.x), static_cast<float>(pos.y) };
}