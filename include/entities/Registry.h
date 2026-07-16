#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "entities/EntityID.h"
#include "entities/ComponentPool.h"
#include "log/Logger.h"

class Registry {
public:
    EntityID create() {
        EntityID id;
        if (!freeList_.empty()) {
            uint32_t idx = freeList_.back();
            freeList_.pop_back();
            id = EntityID{idx, generations_[idx]};
        } else {
            generations_.push_back(0);
            id = EntityID{static_cast<uint32_t>(generations_.size() - 1), 0};
        }
        LOG_INFO("Registry: created entity " + std::to_string(id.index) + " gen " + std::to_string(id.generation));
        return id;
    }

    void destroy(EntityID id) {
        if (!alive(id)) {
            LOG_TRACE("Registry: destroy() called on dead/invalid entity " + std::to_string(id.index));
            return;
        }
        generations_[id.index]++;
        freeList_.push_back(id.index);
        for (auto& [type, pool] : pools_)
            pool->remove(id);
        LOG_INFO("Registry: destroyed entity " + std::to_string(id.index) + " (new gen " + std::to_string(generations_[id.index]) + ")");
    }

    // Called by World::loadMap between maps. Bumps every live generation so
    // any EntityID held from before this call reads as dead rather than
    // aliasing onto whatever gets created next, and drops every pool.
    void clear() {
        LOG_INFO("Registry: clear() — dropping " + std::to_string(pools_.size()) + " pools, " + std::to_string(generations_.size()) + " entity slots");
        for (uint32_t i = 0; i < generations_.size(); ++i)
            generations_[i]++;
        freeList_.clear();
        for (uint32_t i = 0; i < generations_.size(); ++i)
            freeList_.push_back(i);
        pools_.clear();
    }

    bool alive(EntityID id) const {
        return id.index < generations_.size() && generations_[id.index] == id.generation;
    }

    template <typename T, typename... Args>
    T& add(EntityID id, Args&&... args) {
        LOG_TRACE("Registry: add<" + std::string(typeid(T).name()) + "> to entity " + std::to_string(id.index));
        return pool<T>().add(id, std::forward<Args>(args)...);
    }

    template <typename T>
    T* get(EntityID id) {
        auto it = pools_.find(std::type_index(typeid(T)));
        if (it == pools_.end()) return nullptr;
        return static_cast<ComponentPool<T>&>(*it->second).get(id);
    }

    template <typename T>
    bool has(EntityID id) {
        auto it = pools_.find(std::type_index(typeid(T)));
        if (it == pools_.end()) return false;
        return static_cast<ComponentPool<T>&>(*it->second).has(id);
    }

    template <typename T>
    void remove(EntityID id) {
        auto it = pools_.find(std::type_index(typeid(T)));
        if (it != pools_.end()) {
            LOG_TRACE("Registry: remove<" + std::string(typeid(T).name()) + "> from entity " + std::to_string(id.index));
            it->second->remove(id);
        }
    }

    // Iterates the FIRST type's pool, filters by has<Rest>() on the others.
    // Put the rarest component first at the call site for efficiency.
    template <typename First, typename... Rest>
    std::vector<EntityID> view() {
        std::vector<EntityID> result;
        for (auto& [id, _] : pool<First>()) {
            if ((has<Rest>(id) && ...))
                result.push_back(id);
        }
        return result;
    }

private:
    template <typename T>
    ComponentPool<T>& pool() {
        auto key = std::type_index(typeid(T));
        auto it = pools_.find(key);
        if (it == pools_.end())
            it = pools_.emplace(key, std::make_unique<ComponentPool<T>>()).first;
        return static_cast<ComponentPool<T>&>(*it->second);
    }

    std::vector<uint32_t> generations_;
    std::vector<uint32_t> freeList_;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPoolBase>> pools_;
};