#pragma once

#include "ui/UIComponent.h"
#include "ui/UIType.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <memory>
#include <vector>

class UIManager {
public:
    // Add a component; ownership is transferred.
    void addComponent(std::unique_ptr<UIComponent> component) {
        components_.push_back(std::move(component));
    }

    // Optionally: add helpers per type
    template <typename T, typename... Args>
    T& emplaceComponent(UIType type, Args&&... args) {
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        // If T’s ctor doesn’t take a UIType, you can assert that T has a fixed type.
        T& ref = *ptr;
        components_.push_back(std::move(ptr));
        return ref;
    }

    // Draw all components; later you can filter by type or control order.
    void draw(sf::RenderTarget& target) {
        // For now: draw in insertion order; you can sort by z‑order later.
        for (auto& comp : components_) {
            comp->draw(target);
        }
    }

    // If you want separate passes (e.g. only non‑diegetic in a specific SFML view):
    void drawType(sf::RenderTarget& target, UIType type) {
        for (auto& comp : components_) {
            if (comp->type() == type) {
                comp->draw(target);
            }
        }
    }

private:
    std::vector<std::unique_ptr<UIComponent>> components_;
};