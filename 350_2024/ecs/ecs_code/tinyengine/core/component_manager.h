#ifndef TINYENGINE_CORE_COMPONENT_MANAGER_H_
#define TINYENGINE_CORE_COMPONENT_MANAGER_H_

#include "tinyengine/core/component_array.h"
#include "tinyengine/core/types.hpp"

#include <cassert>
#include <memory>
#include <unordered_map>

namespace tinyengine {

class component_type_counter {
    static inline ComponentType current_index = 0;

public:
    template <typename T>
    static inline const ComponentType index_of = current_index++;
};

// Registers and manages a collection of components
class ComponentManager {
public:
    template <typename T>
    void register_component() {
        const auto id = component_type_counter::index_of<T>;
        assert(component_arrays.find(id) == component_arrays.end());
        component_arrays.emplace(id, std::make_shared<ComponentArray<T>>());
    }

    // Get the unique bit idx for the respective component type
    template <typename T>
    auto get_component_type() -> ComponentType {
        const auto id = component_type_counter::index_of<T>;
        assert(component_arrays.find(id) != component_arrays.end());
        return id;
    }

    // Add component to entity
    template <typename T>
    void add_component(Entity entity, T component) {
        get_component_array<T>()->insert(entity, component);
    }

    // Remove component from entity
    template <typename T>
    void remove_component(Entity entity) {
        get_component_array<T>()->remove(entity);
    }

    template <typename T>
    auto get_component(Entity entity) -> T & {
        return get_component_array<T>()->get_entity_data(entity);
    }

    void entity_destroyed(Entity entity) {
        // Notify each component array that an entity has been destroyed
        for (const auto &[_, component_array] : component_arrays) {
            component_array->entity_destroyed(entity);
        }
    }

private:
    template <typename T>
    auto get_component_array() -> std::shared_ptr<ComponentArray<T>> {
        const auto id = component_type_counter::index_of<T>;
        assert(component_arrays.find(id) != component_arrays.end());
        return std::static_pointer_cast<ComponentArray<T>>(component_arrays[id]);
    }
    std::unordered_map<ComponentType, std::shared_ptr<BaseComponentArray>> component_arrays{};
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_COMPONENT_MANAGER_H_
