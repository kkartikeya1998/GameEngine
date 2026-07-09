#pragma once

#include "ui/UIType.h"
#include "render/RenderSystem.h"

class UIComponent {
public:
    explicit UIComponent(UIType type) : type_{type} {}
    virtual ~UIComponent() = default;

    UIType type() const noexcept { return type_; }

    // Render by enqueueing into RenderSystem; no direct SFML dependency here.
    virtual void render(RenderSystem& renderSystem) = 0;

private:
    UIType type_;
};