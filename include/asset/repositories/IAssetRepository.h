#pragma once

// ---------------------------------------------------------------------------
// IAssetRepository — marker base for anything AssetManager can own.
//
// Deliberately empty. The contract is just "derives from this," which lets
// AssetManager store heterogeneous repositories behind one pointer type and
// retrieve them by concrete type via typeid. Loading itself happens in each
// concrete repository's own constructor — this interface doesn't mandate
// *how* a repository loads, only that it can be held polymorphically.
//
// Adding a new asset category (e.g. AnimationRepository) means: derive from
// this, nothing else. AssetManager's public interface never changes.
// ---------------------------------------------------------------------------
class IAssetRepository {
public:
    virtual ~IAssetRepository() = default;
};