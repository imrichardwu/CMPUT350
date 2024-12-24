#ifndef TINYENGINE_CORE_COMPONENT_ARRAY_H_
#define TINYENGINE_CORE_COMPONENT_ARRAY_H_

#include "tinyengine/core/types.hpp"

#include <array>
#include <cassert>
#include <unordered_map>

namespace tinyengine {

class BaseComponentArray {
public:
    BaseComponentArray() = default;
    virtual ~BaseComponentArray() = default;

    BaseComponentArray(const BaseComponentArray &) = default;
    BaseComponentArray(BaseComponentArray &&) = default;
    BaseComponentArray &operator=(const BaseComponentArray &) = default;
    BaseComponentArray &operator=(BaseComponentArray &&) = default;

    // Signal that an entity has been destroyed and it needs to update any mappings
    virtual void entity_destroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public BaseComponentArray {
public:
    void insert(Entity entity, T component) {
        assert(entity_to_index.find(entity) == entity_to_index.end());
        const auto index = entity_to_index.size();
        entity_to_index[entity] = index;
        index_to_entity[index] = entity;
        component_array[index] = std::move(component);
    }

    void remove(Entity entity) {
        assert(entity_to_index.find(entity) != entity_to_index.end());
        // Move element at end to index being removed from to keep packed array
        const auto idx_end = entity_to_index.size() - 1;
        const auto idx_removed = entity_to_index[entity];
        component_array[idx_removed] = component_array[idx_end];

        // Update entity mapping for entity moved from end to removed idx
        const auto entity_to_update = index_to_entity[idx_end];
        entity_to_index[entity_to_update] = idx_removed;
        index_to_entity[idx_removed] = entity_to_update;

        entity_to_index.erase(entity);
        index_to_entity.erase(idx_end);
    }

    auto get_entity_data(Entity entity) -> T & {
        assert(entity_to_index.find(entity) != entity_to_index.end());
        return component_array[entity_to_index[entity]];
    }

    void entity_destroyed(Entity entity) override {
        if (entity_to_index.find(entity) != entity_to_index.end()) {
            return remove(entity);
        }
    }

private:
    std::array<T, MAX_ENTITIES> component_array;
    std::unordered_map<Entity, std::size_t> entity_to_index;
    std::unordered_map<std::size_t, Entity> index_to_entity;
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_COMPONENT_ARRAY_H_
