#include "asset/source/JsonMetadataSource.h"
#include <fstream>

Result<nlohmann::json, AssetError> JsonMetadataSource::load(const std::filesystem::path &path) const
{
    std::ifstream file(path);
    if (!file)
        return Result<nlohmann::json, AssetError>::Err(
            AssetError{AssetErrorCode::FileNotFound, path.string(), "could not open file"});

    nlohmann::json j;
    try
    {
        file >> j;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        // nlohmann throws on malformed JSON regardless of our own error
        // model — this is the one place that exception is allowed to
        // exist, and it dies right here, converted to our vocabulary
        // before it can leak past the format-abstraction boundary.
        return Result<nlohmann::json, AssetError>::Err(
            AssetError{AssetErrorCode::ParseFailure, path.string(), e.what()});
    }

    return Result<nlohmann::json, AssetError>::Ok(std::move(j));
}