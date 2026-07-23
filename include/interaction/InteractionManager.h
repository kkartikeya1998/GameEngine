#pragma once

#include "asset/AsssetPaths.h"
#include "state/DialogueState.h"
#include "state/StateFactory.h"
#include "events/Events.h"
#include "input/InputManager.h"
#include "ui/UIFont.h"
#include "log/Logger.h"
#include "game/GameServices.h"

class InteractionManager
{
public:
    explicit InteractionManager(GameServices services)
        : services_(services) {}

    void HandleRequested(const InteractionRequested &e)
    {
        if (active_)
            return; // ignore re-triggers mid-interaction
        active_ = services_.assets.findInteraction(e.interactionId);
        LOG_TRACE("findInteraction(" + e.interactionId + ") -> " + (active_ ? "FOUND, steps=" + std::to_string(active_->steps.size()) : "NULLPTR"));
        if (!active_ || active_->steps.empty())
        {
            active_ = nullptr;
            return;
        }
        actor_ = e.actor;
        stepIndex_ = 0;
        RunStep(active_->steps[stepIndex_]);
    }

    void HandleDialogueFinished(const DialogueFinished &)
    {
        if (waitingOnState_)
            Advance();
    }
    void HandleBattleFinished(const BattleFinished &)
    {
        if (waitingOnState_)
            Advance();
    }

    void Update(float dt)
    {
        if (!active_ || waitingOnState_)
            return;
        const auto &step = active_->steps[stepIndex_];
        if (step.type == InteractionStepType::Wait)
        {
            waitTimer_ += dt;
            if (waitTimer_ >= step.waitSeconds)
            {
                waitTimer_ = 0.f;
                Advance();
            }
        }
    }

private:
    void Advance()
    {
        waitingOnState_ = false;
        ++stepIndex_;
        if (!active_ || stepIndex_ >= active_->steps.size())
        {
            active_ = nullptr;
            return;
        }
        RunStep(active_->steps[stepIndex_]);
    }

    void RunStep(const InteractionStepData &step)
    {
        LOG_TRACE("RunStep type=" + std::to_string(static_cast<int>(step.type)) + " stepIndex=" + std::to_string(stepIndex_));
        switch (step.type)
        {
        case InteractionStepType::Dialogue:
            services_.states.Push(StateFactory::MakeDialogue(services_, step.text));
            waitingOnState_ = true;
            break;
        case InteractionStepType::Animation:
            // TODO: needs AnimationSystem access for a one-shot clip
            Advance();
            break;
        case InteractionStepType::Battle:
            // TODO: BattleState doesn't exist. Stub fires BattleFinished
            // immediately so the pipeline is provably wired end-to-end.
            services_.events.Push(BattleFinished{});
            waitingOnState_ = true;
            break;
        case InteractionStepType::Wait:
            waitTimer_ = 0.f;
            break;
        case InteractionStepType::GrantItem:
            services_.events.Push(ItemPickedUp{actor_, step.itemId, step.quantity});
            Advance();
            break;
        }
    }

    GameServices services_;
    EntityID actor_{};
    const InteractionAssetMetadata *active_ = nullptr;

    std::size_t stepIndex_ = 0;
    float waitTimer_ = 0.f;
    bool waitingOnState_ = false;
};