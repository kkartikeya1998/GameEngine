#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "entities/Component.h"

// ---------------------------------------------------------------------------
// Entity — a lightweight, open-ended bag of components.
// ---------------------------------------------------------------------------
class Entity {
public:
    Entity() = default;

    // Movable, not copyable — each component is owned by exactly one
    // Entity via unique_ptr<Component>, same single-owner shape as
    // before; only the container changed from a map to a vector.
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    ~Entity() = default;

    template <typename T, typename... Args>
    T& add(Args&&... args)
    {
        // Preserve "exactly one T per Entity" — erase any existing T
        // first, same end result the old map's operator[]= overwrite
        // gave for free. Done before constructing the new one so a
        // throwing constructor doesn't leave the entity without its
        // previous T.
        remove<T>();

        auto owned = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *owned;
        components_.push_back(std::move(owned));
        return ref;
    }

    template <typename T>
    T* get()
    {
        for (auto& c : components_) {
            if (auto* t = dynamic_cast<T*>(c.get()))
                return t;
        }
        return nullptr;
    }

    template <typename T>
    const T* get() const
    {
        for (const auto& c : components_) {
            if (auto* t = dynamic_cast<const T*>(c.get()))
                return t;
        }
        return nullptr;
    }

    template <typename T>
    bool has() const
    {
        return get<T>() != nullptr;
    }

    template <typename T>
    void remove()
    {
        for (auto it = components_.begin(); it != components_.end(); ++it) {
            if (dynamic_cast<T*>(it->get())) {
                components_.erase(it);
                // Stops at the first match — add<T>()'s own
                // single-T-per-Entity invariant means there should
                // never be more than one to find, but this loop
                // doesn't itself assume that; it just removes the
                // first T it sees, same as the old map's single-slot
                // erase did.
                return;
            }
        }
    }

    // Exposes every component on this entity as base-class pointers,
    // for callers that need to iterate without knowing concrete types
    // up front (e.g. World::view<T>() built on top of this. 
    // Returns non-owning raw pointers; lifetime stays tied
    // to this Entity.
    std::vector<Component*> getAllComponents()
    {
        std::vector<Component*> result;
        result.reserve(components_.size());
        for (auto& c : components_)
            result.push_back(c.get());
        return result;
    }

private:
    std::vector<std::unique_ptr<Component>> components_;
};