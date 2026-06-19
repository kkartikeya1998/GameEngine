#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "world/MapObject.h"

// ---------------------------------------------------------------------------
// MapObjectRepository — owns all object type metadata, loaded once from
// object_metadata.json at game startup.
//
// MapLoader looks up each placed instance's "type" string here to get the
// footprint/texture, rather than duplicating that data in every map file.
//
// Ownership:
//   Repository owns all ObjectTypeMetadata. Returned pointers are non-owning
//   and valid for the repository's lifetime — load it once and keep it alive
//   for as long as any Map built from it is in use.
// ---------------------------------------------------------------------------
class MapObjectRepository {
public:
    explicit MapObjectRepository(const std::string& metadataFilePath);

    // Returns nullptr if the type name isn't in the catalog.
    const ObjectTypeMetadata* find(const std::string& type) const;
    void register_type(const std::string& name, ObjectTypeMetadata meta);

private:
    std::unordered_map<std::string, ObjectTypeMetadata> types_;

    void load_from_file(const std::string& path);
};