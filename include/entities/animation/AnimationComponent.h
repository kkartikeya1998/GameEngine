#pragma once

class AnimationComponent {
public:
    explicit AnimationComponent(float speed = 6.0f);

    // start movement animation
    void begin(float startX, float startY, float targetX, float targetY);

    // called every frame
    void update(float dt);

    bool isAnimating() const;

    float getRenderX() const;
    float getRenderY() const;

private:
    float startX_;
    float startY_;

    float targetX_;
    float targetY_;

    float currentX_;
    float currentY_;

    float progress_ = 1.0f;
    float speed_;
};