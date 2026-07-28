#pragma once
#include <string>

// ---------------------------------------------------------------------------
// AnimationComponent — explicit animation state for entities driven
// directly by AnimationAssetMetadata (walk/idle/sprint/etc.)
//
// When present on an entity, AnimationSystem uses this instead of
// WalkCycleTimer. currentAnimation is the id suffix appended to the
// character name the same way WalkCycleTimer's state string is today
// (e.g. characterName + "_" + "attack").
// ---------------------------------------------------------------------------
struct AnimationComponent {
    std::string currentAnimation = "walk";
    int currentFrame = 0;
    float elapsed = 0.f;
    bool playing = true;
    bool finished = false; // true once a non-looping animation completes

    AnimationComponent() = default;
    explicit AnimationComponent(std::string animation)
        : currentAnimation(std::move(animation)) {}

    void play(const std::string& animation) {
        if (currentAnimation == animation && playing) return;
        currentAnimation = animation;
        currentFrame = 0;
        elapsed = 0.f;
        playing = true;
        finished = false;
    }
};