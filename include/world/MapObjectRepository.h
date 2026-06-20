#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "world/MapObject.h"

// ---------------------------------------------------------------------------
// MapObjectRepository — owns all object type metadata.
//
// Loading is now triggered explicitly via load_from_file(), called once by
// MapLoader's constructor. This keeps the parsing logic in one place
// (it used to be duplicated in MapLoader::loadObjectMetadata — removed).
// ---------------------------------------------------------------------------
class MapObjectRepository {
public:
    MapObjectRepository() = default;

    void load_from_file(const std::string& path);
    void register_type(const std::string& typeName, ObjectTypeMetadata meta);

    const ObjectTypeMetadata* find(const std::string& type) const;

private:
    std::unordered_map<std::string, ObjectTypeMetadata> types_;
};