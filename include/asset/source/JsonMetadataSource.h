#pragma once
#include "asset/source/IMetadataSource.h"

class JsonMetadataSource : public IMetadataSource {
public:
    nlohmann::json load(const std::filesystem::path& path) const override;
};