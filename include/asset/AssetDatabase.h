#pragma once
#include <memory>
#include <vector>
#include "asset/source/IMetadataSource.h"
#include "asset/repositories/ComponentAssetRepository.h"
#include "asset/metadata/RenderAssetMetadata.h"
#include "asset/metadata/CollisionAssetMetadata.h"
#include "asset/metadata/AnimationAssetMetadata.h"
#include "asset/metadata/ArchetypeDefinition.h"

class AssetDatabase
{
public:
    AssetDatabase();
    void loadFile(const std::filesystem::path &path, const IMetadataSource &source);

    // Call once after all loadFile() calls.
    void finalize();

    const RenderAssetMetadata *findRender(const std::string &id) const { return renderRepo_->find(id); }
    const CollisionAssetMetadata *findCollision(const std::string &id) const { return collisionRepo_->find(id); }
    const AnimationAssetMetadata *findAnimation(const std::string &id) const { return animationRepo_->find(id); }
    const ArchetypeDefinition *findArchetype(const std::string &id) const { return archetypeRepo_->find(id); }
    const ComponentAssetRepository<RenderAssetMetadata> &renderRepository() const { return *renderRepo_; }

private:
    nlohmann::json renderSection_ = nlohmann::json::object();
    nlohmann::json collisionSection_ = nlohmann::json::object();
    nlohmann::json animationSection_ = nlohmann::json::object();
    nlohmann::json archetypeSection_ = nlohmann::json::object();

    std::unique_ptr<ComponentAssetRepository<RenderAssetMetadata>> renderRepo_;
    std::unique_ptr<ComponentAssetRepository<CollisionAssetMetadata>> collisionRepo_;
    std::unique_ptr<ComponentAssetRepository<AnimationAssetMetadata>> animationRepo_;
    std::unique_ptr<ComponentAssetRepository<ArchetypeDefinition>> archetypeRepo_;
};