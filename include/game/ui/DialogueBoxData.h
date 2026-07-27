#pragma once
#include <string>

// Plain data for a dialogue box
struct DialogueBoxData {
    std::string speaker; // empty if unused
    std::string text;
    float x = 0.f;
    float y = 0.f;
    float width = 0.f;
    float height = 0.f;
};