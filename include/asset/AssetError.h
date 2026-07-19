#pragma once
#include <format>
#include <string>

// Recoverable asset-resolution failures. Every runtime asset/entity lookup
// that can plausibly miss (bad map data, a modder's typo, a missing
// sprite reference) returns Result<T, AssetError> — see exceptions/Result.h —
// instead of throwing or returning a bare nullptr for the caller to
// forget to check. Startup-mandatory loads still throw AssetLoadException
// (exceptions/EngineExceptions.h); this is for everything after that boundary.
enum class AssetErrorCode
{
    FileNotFound,
    ParseFailure,
    UnknownId,
    DuplicateId,
    SchemaMismatch,
};

struct AssetError
{
    AssetErrorCode code;
    std::string assetId;  // the id/path being resolved when the failure occurred
    std::string context;  // free-text detail, e.g. an underlying parser message

    std::string toString() const
    {
        static constexpr const char *names[] = {
            "FileNotFound", "ParseFailure", "UnknownId", "DuplicateId", "SchemaMismatch"};
        return std::format("AssetError[{}] id='{}' — {}",
                            names[static_cast<int>(code)], assetId, context);
    }
};