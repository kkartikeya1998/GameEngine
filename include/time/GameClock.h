#pragma once

#include <cstdint>
#include <algorithm>

// Single authoritative source of timing information for the engine.
// Nothing else should touch the OS clock directly.
class GameClock
{
public:
    explicit GameClock(double fixedHz = 60.0)
        : fixedDeltaTime_(1.0 / fixedHz) {}

    // Called once per frame by Game::Run with the raw wall-clock delta.
    void BeginFrame(double rawDeltaSeconds)
    {
        ++frameNumber_;

        unscaledDeltaTime_ = ClampSpike(rawDeltaSeconds);
        deltaTime_ = unscaledDeltaTime_ * timeScale_;
        totalTime_ += deltaTime_;

        accumulator_ += deltaTime_;
    }

    bool ShouldRunFixedUpdate() const { return accumulator_ >= fixedDeltaTime_; }

    void ConsumeFixedStep()
    {
        accumulator_ -= fixedDeltaTime_;
        ++fixedFrameNumber_;
    }

    // Call after the fixed-update loop is drained, before Render.
    void EndFrame()
    {
        interpolationAlpha_ = accumulator_ / fixedDeltaTime_;
    }

    uint64_t FrameNumber() const      { return frameNumber_; }
    uint64_t FixedFrameNumber() const { return fixedFrameNumber_; }
    double DeltaTime() const          { return deltaTime_; }
    double UnscaledDeltaTime() const  { return unscaledDeltaTime_; }
    double TotalTime() const          { return totalTime_; }
    double FixedDeltaTime() const     { return fixedDeltaTime_; }
    double TimeScale() const          { return timeScale_; }
    double InterpolationAlpha() const { return interpolationAlpha_; }

    void SetTimeScale(double scale) { timeScale_ = scale; }

private:
    static double ClampSpike(double raw)
    {
        constexpr double kMaxFrameTime = 0.25; // guard against debugger pauses / alt-tab spikes
        return std::min(raw, kMaxFrameTime);
    }

    uint64_t frameNumber_ = 0;
    uint64_t fixedFrameNumber_ = 0;

    // Note: deltaTime_ already has timeScale_ applied before it's added to the accumulator,
    // so a future slow-mo effect would also slow the fixed simulation step.
    // If we want physics to stay locked to real time regardless of slow-mo,
    // the accumulator should instead be fed unscaledDeltaTime_
    double deltaTime_ = 0.0;
    double unscaledDeltaTime_ = 0.0;
    double totalTime_ = 0.0;
    double timeScale_ = 1.0;

    double fixedDeltaTime_;
    double accumulator_ = 0.0;
    double interpolationAlpha_ = 0.0;
};