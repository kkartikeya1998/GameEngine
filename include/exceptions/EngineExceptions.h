#pragma once
#include <stdexcept>
#include <string>

// Unrecoverable engine failures. These propagate to a single boundary
// (main.cpp) which logs and terminates cleanly — see main.cpp and the note
// on Game::Run() below. Never thrown *or caught* mid-frame: if you're
// tempted to catch one inside Game::Update or a system, the condition
// you're actually handling is either a programmer error (ENGINE_ASSERT) or
// recoverable (Result<T,E>), not this.
class EngineException : public std::runtime_error
{
public:
    explicit EngineException(const std::string &what) : std::runtime_error(what) {}
};

// Mandatory startup asset failed to load (metadata file missing/malformed,
// or contains a duplicate id). Thrown only from AssetDatabase's constructor
// / loadFile — a runtime lookup miss during gameplay is a *different*
// condition and returns Result<T, AssetError> instead (asset/AssetError.h).
class AssetLoadException : public EngineException
{
public:
    explicit AssetLoadException(const std::string &what) : EngineException(what) {}
};

// Window/graphics-context creation failed (no display, driver issue,
// headless environment). There is no meaningful way to continue without a
// renderer, so this — like AssetLoadException — is thrown once and left to
// propagate to the single top-level boundary in main.cpp.
class RendererInitException : public EngineException
{
public:
    explicit RendererInitException(const std::string &what) : EngineException(what) {}
};