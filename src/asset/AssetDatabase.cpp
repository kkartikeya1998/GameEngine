#include "asset/source/JsonMetadataSource.h"
#include "asset/AssetDatabase.h"
#include "asset/AsssetPaths.h"
#include "log/Logger.h"
#include <stdexcept>

namespace
{
    // File-level "defaults" (e.g. shared texture path) get merged into
    // every entry in a section before that entry is handed to fromJson,
    // so per-entry overrides still win. Mirrors what MapObjectRepository
    // used to do inline with j.value("texture", "").
    void applyDefaults(nlohmann::json &section, const nlohmann::json &defaults)
    {
        for (auto &[id, entry] : section.items())
        {
            nlohmann::json merged = defaults;
            merged.merge_patch(entry);
            entry = merged;
        }
    }

    void accumulate(nlohmann::json &dest, const nlohmann::json &src)
    {
        for (auto it = src.begin(); it != src.end(); ++it)
        {
            if (dest.contains(it.key()))
                throw std::runtime_error("AssetDatabase: duplicate asset id: " + it.key());
            dest[it.key()] = it.value();
        }
    }
}

AssetDatabase::AssetDatabase()
{
    LOG_INFO("Initializing Asset Database...");
    JsonMetadataSource jsonSource;

    loadFile(Assets::Maps::OBJECT_METADATA, jsonSource);
    loadFile(Assets::Maps::TILESET_METADATA, jsonSource);
    loadFile(Assets::Items::ITEM_METADATA, jsonSource);

    loadFile(Assets::Characters::CHARACTERS_METADATA, jsonSource);
    loadFile(Assets::Characters::SPECIES_METADATA, jsonSource);

    finalize();
}

void AssetDatabase::loadFile(const std::filesystem::path &path, const IMetadataSource &source)
{
    LOG_DEBUG(std::format("Loading metadata file: {}", path.string()));
    nlohmann::json root = source.load(path);
    nlohmann::json defaults = root.value("defaults", nlohmann::json::object());

    if (root.contains("components"))
    {
        auto &components = root["components"];

        if (components.contains("Render"))
        {
            auto section = components["Render"];
            applyDefaults(section, defaults);
            accumulate(renderSection_, section);
        }
        if (components.contains("Collision"))
        {
            accumulate(collisionSection_, components["Collision"]);
        }
        if (components.contains("Animation"))
        {
            auto section = components["Animation"];
            applyDefaults(section, defaults);
            accumulate(animationSection_, section);
        }

        if (components.contains("PmdAnimation"))
        {
            accumulate(
                pmdAnimationSection_,
                components["PmdAnimation"]);
        }

        if (components.contains("Interaction"))
        {
            accumulate(interactionSection_, components["Interaction"]);
        }

        if (components.contains("Item"))
        {
            accumulate(itemSection_, components["Item"]);
        }
    }

    if (root.contains("archetypes"))
        accumulate(archetypeSection_, root["archetypes"]);
}

void AssetDatabase::finalize()
{
    LOG_INFO("Finalizing Asset Database repositories...");
    archetypeRepo_ = std::make_unique<ComponentAssetRepository<ArchetypeDefinition>>(archetypeSection_);
    renderRepo_ = std::make_unique<ComponentAssetRepository<RenderAssetMetadata>>(renderSection_);
    collisionRepo_ = std::make_unique<ComponentAssetRepository<CollisionAssetMetadata>>(collisionSection_);
    animationRepo_ = std::make_unique<ComponentAssetRepository<AnimationAssetMetadata>>(animationSection_);
    pmdAnimationRepo_ = std::make_unique<ComponentAssetRepository<PmdAnimationSetMetadata>>(pmdAnimationSection_);
    interactionRepo_ = std::make_unique<ComponentAssetRepository<InteractionAssetMetadata>>(interactionSection_);
    itemRepo_ = std::make_unique<ComponentAssetRepository<ItemMetadata>>(itemSection_);
}