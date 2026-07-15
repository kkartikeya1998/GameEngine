#pragma once

#include <unordered_map>
#include "entities/EntityID.h"

// Polymorphic base so Registry can hold heterogeneous pools behind one
// container and still clean up on destroy() without knowing concrete types.
class IComponentPoolBase {
public:
    virtual ~IComponentPoolBase() = default;
    virtual void remove(EntityID id) = 0;
};

template <typename T>
class ComponentPool : public IComponentPoolBase {
public:
    template <typename... Args>
    T& add(EntityID id, Args&&... args) {
        auto [it, _] = data_.insert_or_assign(id, T(std::forward<Args>(args)...));
        return it->second;
    }

    T* get(EntityID id) {
        auto it = data_.find(id);
        return it == data_.end() ? nullptr : &it->second;
    }

    bool has(EntityID id) const { return data_.count(id) != 0; }
    void remove(EntityID id) override { data_.erase(id); }

    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }
    std::size_t size() const { return data_.size(); }

private:
    std::unordered_map<EntityID, T, EntityIDHash> data_;
};