#pragma once
#include "engine/ecs/Component.h"
#include <string>

struct InteractableComponent : public Component {
    std::string interactionId;
    InteractableComponent() = default;
    explicit InteractableComponent(std::string interactionId) : interactionId(std::move(interactionId)) {}
};