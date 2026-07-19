#pragma once

#include <optional>
#include <string>
#include <SFML/Window/Event.hpp>
#include "component/RenderComponent.h"
#include "render/Camera.h"

// Where renderX/renderY are measured from for a given RenderComponent
enum class RenderAnchor
{
    TopLeft,
    CenterBottom
};

// ---------------------------------------------------------------------------
// IRenderer — abstract interface for rendering.
//
// The renderer doesn't know or care about World, Player, or GameController.
// It only knows how to draw a fully-resolved RenderComponent and debug
// primitives. Name/texture resolution happens upstream in RenderSystem.
//
// Error-handling contract for implementers (see SFMLRenderer for the
// reference implementation):
//   - Failure to acquire a graphics context/window is unrecoverable and
//     should be signaled by throwing from the constructor (a
//     RendererInitException, exceptions/EngineExceptions.h) — never from
//     drawEntity() or any per-frame method.
//   - A RenderComponent that references a resource which fails to resolve
//     at draw time (bad texture path, etc.) is a recoverable, content-level
//     condition. drawEntity() must not throw for this — degrade visibly
//     (e.g. a placeholder sprite) and log at most once per unique failing
//     reference, not once per frame.
// ---------------------------------------------------------------------------
class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void clear() = 0;
    virtual void beginWorldView(const Camera &camera) = 0;
    virtual void setDefaultView() = 0; // default view for UI components

    // Single draw path for tiles, players, and map objects alike
    virtual void drawEntity(const RenderComponent &render, RenderAnchor anchor) = 0;

    // Debug-only: draws an unfilled rectangle outline at world-pixel coordinates
    virtual void drawDebugRect(float x, float y, float width, float height) = 0;

    // Filled, colored rect. screenSpace=true draws in window coordinates,
    // ignoring the active camera view (e.g. fullscreen overlays).
    virtual void drawRect(float x, float y, float width, float height,
                          sf::Color color, bool screenSpace) = 0;

    virtual void drawText(const sf::Text &text) = 0;

    virtual void present() = 0;
    virtual bool isOpen() const = 0;
    virtual std::optional<sf::Event> pollEvent() = 0;
};