#include "asset/metadata/InteractionAssetMetadata.h"

namespace {
    InteractionStepType parseType(const std::string& s) {
        if (s == "dialogue") return InteractionStepType::Dialogue;
        if (s == "animation") return InteractionStepType::Animation;
        if (s == "battle") return InteractionStepType::Battle;
        if (s == "wait") return InteractionStepType::Wait;
        throw std::runtime_error("InteractionAssetMetadata: unknown step type: " + s);
    }
}

InteractionAssetMetadata InteractionAssetMetadata::fromJson(const std::string& id, const nlohmann::json& entry)
{
    InteractionAssetMetadata meta;
    meta.name = id;
    for (const auto& stepJson : entry.at("steps")) {
        InteractionStepData step;
        step.type = parseType(stepJson.at("type").get<std::string>());
        step.text = stepJson.value("text", "");
        step.animationId = stepJson.value("animationId", "");
        step.targetEntity = stepJson.value("targetEntity", "");
        step.trainerId = stepJson.value("trainerId", "");
        step.waitSeconds = stepJson.value("waitSeconds", 0.f);
        meta.steps.push_back(std::move(step));
    }
    return meta;
}