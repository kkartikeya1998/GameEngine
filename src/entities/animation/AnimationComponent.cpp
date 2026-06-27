#include <algorithm>
#include <iostream>

#include "entities/animation/AnimationComponent.h"

AnimationComponent::AnimationComponent(float speed)
    : speed_(speed)
{
}

void AnimationComponent::begin(
    float startX,
    float startY,
    float targetX,
    float targetY)
{
    startX_ = startX;
    startY_ = startY;

    targetX_ = targetX;
    targetY_ = targetY;

    currentX_ = startX;
    currentY_ = startY;

    progress_ = 0.f;
}

void AnimationComponent::update(float dt)
{
    // std::cout << "Animation progress: " << progress_ << ", currentX: " << currentX_ << ", currentY: " << currentY_ << std::endl;
    if (progress_ >= 1.f)
        return;

    progress_ += speed_ * dt;

    if (progress_ > 1.f)
        progress_ = 1.f;

    currentX_ = startX_ + (targetX_ - startX_) * progress_;

    currentY_ = startY_ + (targetY_ - startY_) * progress_;

}

bool AnimationComponent::isAnimating() const
{
    return progress_ < 1.f;
}

float AnimationComponent::getProgress() const
{
    return progress_;
}

float AnimationComponent::getRenderX() const
{
    return currentX_;
}

float AnimationComponent::getRenderY() const
{
    return currentY_;
}