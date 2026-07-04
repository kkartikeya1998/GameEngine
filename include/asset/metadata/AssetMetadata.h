#pragma once

#include <string>
// the name field is used for lookup in the repository, so it must be unique per asset type
struct AssetMetadata {
    std::string name;
};