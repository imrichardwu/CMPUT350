#ifndef TINYENGINE_CORE_COORDINATOR_H_
#define TINYENGINE_CORE_COORDINATOR_H_

#include "tinyengine/core/component_manager.h"
#include "tinyengine/core/entity_manager.h"
#include "tinyengine/core/system_manager.h"

#include <any>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace tinyengine {

class Coordinator {
public:
    Coordinator()
        : component_manager(std::make_unique<ComponentManager>()),
          entity_manager(std::make_unique<EntityManager>()),
          system_manager(std::make_unique<SystemManager>()) {}

    // Entity methods
    auto create_entity() -> Entity {
        return entity_manager->create_entity();
    }

    void destroy_entity(Entity entity) {
        entity_manager->destroy_entity(entity);
        component_manager->entity_destroyed(entity);
        system_manager->entity_dstroyed(entity);
    }

    // Component methods
    template <typename T>
    void register_component() {
        component_manager->register_component<T>();
    }

    // Add component to an entity
    template <typename T>
    void add_component(Entity entity, T component) {
        // Add entity to manager for that component
        component_manager->add_component<T>(entity, component);

        // Upate signature bit to reflect added component
        auto signature = entity_manager->get_entity_signature(entity);
        signature.set(component_manager->get_component_type<T>(), true);

        // Notify signature change since entity is given component
        entity_manager->set_entity_signature(entity, signature);
        system_manager->entity_signature_changed(entity, signature);
    }

    // Remove component from an entity
    template <typename T>
    void remove_component(Entity entity) {
        component_manager->remove_component<T>(entity);

        // Upate signature bit to reflect removed component
        auto signature = entity_manager->get_entity_signature(entity);
        signature.set(component_manager->get_component_type<T>(), false);

        // Notify signature change since entity is given component
        entity_manager->set_entity_signature(entity, signature);
        system_manager->entity_signature_changed(entity, signature);
    }

    template <typename T>
    auto get_component(Entity entity) -> T & {
        return component_manager->get_component<T>(entity);
    }

    template <typename T>
    auto get_component_type() -> ComponentType {
        return component_manager->get_component_type<T>();
    }

    // System methods
    template <typename T, typename... Args>
    auto register_system(int priority, Args... args) -> std::shared_ptr<T> {
        return system_manager->register_system<T>(priority, args...);
    }

    template <typename T>
    void set_system_signature(Signature signature) {
        system_manager->set_signature<T>(std::move(signature));
    }

    void update_all_standard_systems(double step_seconds) {
        system_manager->update_all_standard(*this, step_seconds);
    }

    void update_all_render_systems(double step_seconds) {
        system_manager->update_all_render(*this, step_seconds);
    }

    // Misc
    void store_data(const std::string &name, std::any value) {
        data[name] = value;
    }

    template <typename T>
    auto get_data(const std::string &name) -> T & {
        using U = std::remove_cv_t<std::remove_reference_t<T>>;
        return std::any_cast<U &>(data[name]);
    }

private:
    std::unique_ptr<ComponentManager> component_manager;
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<SystemManager> system_manager;
    std::unordered_map<std::string, std::any> data;
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_COORDINATOR_H_
