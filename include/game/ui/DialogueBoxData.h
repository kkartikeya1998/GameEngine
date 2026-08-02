#pragma once
#include <string>
#include "game/ui/UILayout.h"

// Plain data for a dialogue box
struct DialogueBoxData {
    std::string speaker; // empty if unused
    std::string text;
    UILayout layout; // resolved to a UIRect at render time by UILayoutResolver
};