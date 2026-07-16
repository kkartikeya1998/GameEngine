#pragma once
#include "asset/AssetDatabase.h"
#include "state/StateMachine.h"
#include "state/IGameState.h"
#include "state/DialogueState.h"
#include "events/EventQueue.h"
#include "events/Events.h"
#include "input/InputManager.h"

class InteractionManager {
public:
    InteractionManager(AssetDatabase& assets, StateMachine<IGameState>& stateMachine, InputManager& input, EventQueue& events)
        : assets_(assets), stateMachine_(stateMachine), input_(input), events_(events) {}

    void HandleRequested(const InteractionRequested& e)
    {
        if (active_) return; // ignore re-triggers mid-interaction
        active_ = assets_.findInteraction(e.interactionId);
        if (!active_ || active_->steps.empty()) { active_ = nullptr; return; }
        stepIndex_ = 0;
        RunStep(active_->steps[stepIndex_]);
    }

    void HandleDialogueFinished(const DialogueFinished&) { if (waitingOnState_) Advance(); }
    void HandleBattleFinished(const BattleFinished&)     { if (waitingOnState_) Advance(); }

    void Update(float dt)
    {
        if (!active_ || waitingOnState_) return;
        const auto& step = active_->steps[stepIndex_];
        if (step.type == InteractionStepType::Wait) {
            waitTimer_ += dt;
            if (waitTimer_ >= step.waitSeconds) { waitTimer_ = 0.f; Advance(); }
        }
    }

private:
    void Advance()
    {
        waitingOnState_ = false;
        ++stepIndex_;
        if (!active_ || stepIndex_ >= active_->steps.size()) { active_ = nullptr; return; }
        RunStep(active_->steps[stepIndex_]);
    }

    void RunStep(const InteractionStepData& step)
    {
        switch (step.type) {
            case InteractionStepType::Dialogue:
                stateMachine_.Push(std::make_unique<DialogueState>(input_, stateMachine_, events_, step.text));
                waitingOnState_ = true;
                break;
            case InteractionStepType::Animation:
                // TODO: needs AnimationSystem access for a one-shot clip — deferred,
                // per your notes AnimationPlayerComponent integration is unresolved.
                Advance();
                break;
            case InteractionStepType::Battle:
                // TODO: BattleState doesn't exist. Stub fires BattleFinished
                // immediately so the pipeline is provably wired end-to-end.
                events_.Push(BattleFinished{});
                waitingOnState_ = true;
                break;
            case InteractionStepType::Wait:
                waitTimer_ = 0.f;
                break;
        }
    }

    AssetDatabase& assets_;
    StateMachine<IGameState>& stateMachine_;
    InputManager& input_;
    EventQueue& events_;
    const InteractionAssetMetadata* active_ = nullptr;
    std::size_t stepIndex_ = 0;
    float waitTimer_ = 0.f;
    bool waitingOnState_ = false;
};