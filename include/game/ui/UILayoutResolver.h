#pragma once
#include "game/ui/UILayout.h"
#include "game/GameConstants.h" // ASSUMPTION on include path — adjust to match your tree

// Stateless conversion of a UILayout into absolute screen-space pixels.
// Pure function: same layout + bounds always produce the same UIRect.
namespace UILayoutResolver
{
    inline UIRect Resolve(const UILayout &layout,
                           UIRect screenBounds = {0.f, 0.f,
                                                   static_cast<float>(GameConstants::GAME_RESOLUTION_W),
                                                   static_cast<float>(GameConstants::GAME_RESOLUTION_H)})
    {
        UIRect rect;

        // Size first — anchoring math needs it for Center/Right/Bottom.
        if (layout.size.mode == UISizeMode::Percent)
        {
            rect.width = screenBounds.width * layout.size.value.x;
            rect.height = screenBounds.height * layout.size.value.y;
        }
        else // Fixed
        {
            rect.width = layout.size.value.x;
            rect.height = layout.size.value.y;
        }

        switch (layout.h)
        {
            case HorizontalAnchor::Left:
                rect.x = screenBounds.x;
                break;
            case HorizontalAnchor::Center:
                rect.x = screenBounds.x + (screenBounds.width - rect.width) * 0.5f;
                break;
            case HorizontalAnchor::Right:
                rect.x = screenBounds.x + screenBounds.width - rect.width;
                break;
        }

        switch (layout.v)
        {
            case VerticalAnchor::Top:
                rect.y = screenBounds.y;
                break;
            case VerticalAnchor::Center:
                rect.y = screenBounds.y + (screenBounds.height - rect.height) * 0.5f;
                break;
            case VerticalAnchor::Bottom:
                rect.y = screenBounds.y + screenBounds.height - rect.height;
                break;
        }

        rect.x += layout.offset.x;
        rect.y += layout.offset.y;

        return rect;
    }
}