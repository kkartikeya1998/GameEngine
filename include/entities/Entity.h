#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

// ---------------------------------------------------------------------------
// Entity — a lightweight, open-ended bag of components, keyed by type.
//
// REPLACES the previous Entity, which hardcoded exactly two named
// strategy slots (IMovementBehavior, IRenderState). That shape is what
// made `Player` need to be a type alias (no room to attach
// player-only data) and made `MapObject` unable to become an Entity
// (no movement to inject, but the old constructor required one).
//
// Entity itself holds NO behavior and NO movement/render logic — those
// now live in free functions / Systems (MovementSystem, RenderSystem)
// that take an Entity& and look up whichever components they need via
// get<T>(). Components are pure data (see GridMovementComponent,
// FreeMovementComponent, GridRenderComponent, FreeRenderComponent,
// PlayerControlComponent, NpcComponent, etc.) — the logic that used to
// live inside IMovementBehavior/IRenderState implementations now lives
// in MovementSystem/RenderSystem instead.
//
// add<T>() constructs and stores exactly one T per Entity — adding a
// second T overwrites/destroys the first, same as an Entity never
// having had two IMovementBehaviors before. Need two conceptually
// different "movement kinds" on the table at once? That's two
// distinct component TYPES (GridMovementComponent vs
// FreeMovementComponent), not two instances of one type.
//
// get<T>() returns nullptr if the component isn't present. Systems are
// expected to check before using:
//
//   if (auto* m = entity.get<FreeMovementComponent>()) { ... }
//
// This is what lets MapObject skip movement entirely, and lets
// Player/NPC share the same systems despite having different
// movement-component types attached.
// ---------------------------------------------------------------------------
class Entity {
public:
    Entity() = default;

    // Movable, not copyable — each component is owned via a type-erased
    // owning slot (IComponentHolder below), same single-owner shape the
    // old unique_ptr<IMovementBehavior>/<IRenderState> members had.
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    ~Entity() = default;

    template <typename T, typename... Args>
    T& add(Args&&... args)
    {
        auto owned = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *owned;
        components_[std::type_index(typeid(T))] =
            std::make_unique<ComponentHolder<T>>(std::move(owned));
        return ref;
    }

    template <typename T>
    T* get()
    {
        auto it = components_.find(std::type_index(typeid(T)));
        if (it == components_.end())
            return nullptr;
        return static_cast<ComponentHolder<T>*>(it->second.get())->value.get();
    }

    template <typename T>
    const T* get() const
    {
        auto it = components_.find(std::type_index(typeid(T)));
        if (it == components_.end())
            return nullptr;
        return static_cast<const ComponentHolder<T>*>(it->second.get())->value.get();
    }

    template <typename T>
    bool has() const
    {
        return components_.find(std::type_index(typeid(T))) != components_.end();
    }

    template <typename T>
    void remove()
    {
        components_.erase(std::type_index(typeid(T)));
    }

private:
    // Type-erased base so ~IComponentHolder() virtual-dispatches to the
    // right ~ComponentHolder<T>(), which destroys the right T — this is
    // the standard "type-erased owning slot" pattern, and the whole
    // reason unique_ptr<void> alone doesn't work here (void has no
    // destructor to dispatch to).
    struct IComponentHolder {
        virtual ~IComponentHolder() = default;
    };

    template <typename T>
    struct ComponentHolder : IComponentHolder {
        explicit ComponentHolder(std::unique_ptr<T> v) : value(std::move(v)) {}
        std::unique_ptr<T> value;
    };

    std::unordered_map<std::type_index, std::unique_ptr<IComponentHolder>> components_;
};