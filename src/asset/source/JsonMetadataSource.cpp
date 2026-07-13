#include "asset/source/JsonMetadataSource.h"
#include <fstream>
#include <stdexcept>

nlohmann::json JsonMetadataSource::load(const std::filesystem::path& path) const
{
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("JsonMetadataSource: failed to open " + path.string());

    nlohmann::json j;
    file >> j;
    return j;
}