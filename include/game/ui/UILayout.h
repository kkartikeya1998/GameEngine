#pragma once

// Screen anchors for UI elements, independent of pixel/resolution specifics.
enum class HorizontalAnchor { Left, Center, Right };
enum class VerticalAnchor   { Top, Center, Bottom };

enum class UISizeMode { Fixed, Percent };

// Fixed: value is pixels. Percent: value is a 0..1 fraction of the screen bounds.
struct UISize
{
    UISizeMode mode = UISizeMode::Fixed;
    struct { float x = 0.f; float y = 0.f; } value;
};

// Applied after anchoring — nudges the resolved rect by a fixed pixel amount.
struct UIPadding
{
    float x = 0.f;
    float y = 0.f;
};

// Declarative layout: how to anchor + size a UI element, resolved later
// against actual screen bounds by UILayoutResolver. No raw x/y/width/height here.
struct UILayout
{
    HorizontalAnchor h = HorizontalAnchor::Center;
    VerticalAnchor v = VerticalAnchor::Center;
    UISize size;
    UIPadding offset;
};

// Final resolved screen-space rectangle, ready for RenderSystem submission.
struct UIRect
{
    float x = 0.f;
    float y = 0.f;
    float width = 0.f;
    float height = 0.f;
};