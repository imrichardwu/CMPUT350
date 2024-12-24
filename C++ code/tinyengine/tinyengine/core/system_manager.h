#ifndef TINYENGINE_CORE_SYSTEM_MANAGER_H_
#define TINYENGINE_CORE_SYSTEM_MANAGER_H_

#include "tinyengine/core/system.h"
#include "tinyengine/core/types.hpp"

#include <cassert>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace tinyengine {

// Forward declare to avoid recursive includes
class Coordinator;

class system_type_counter {
    static inline std::size_t current_index = 0;

public:
    template <typename T>
    static inline const std::size_t index_of = current_index++;
};

class SystemManager {
public:
    template <typename T, typename... Args>
    auto register_system(int priority, Args... args) -> std::shared_ptr<T> {
        const auto idx = system_type_counter::index_of<T>;
        assert(systems.find(idx) == systems.end());

        // Create system and give to caller to use externally
        auto system = std::make_shared<T>(args...);
        systems_by_priority[priority].push_back(system);
        systems.emplace(idx, system);
        return system;
    }

    template <typename T>
    void set_signature(Signature signature) {
        const auto idx = system_type_counter::index_of<T>;
        assert(systems.find(idx) != systems.end());
        signatures[idx] = std::move(signature);
    }

    // Remove from all system lists
    void entity_dstroyed(Entity entity) {
        for (const auto &[_, system] : systems) {
            system->entities.erase(entity);
        }
    }

    // Update signature for each system
    void entity_signature_changed(Entity entity, Signature signature) {
        for (const auto &[type_name, system] : systems) {
            const auto &system_signature = signatures[type_name];
            // If system signature matches new signature (no change), keep/add entity
            // Otherwise, new signature doesn't match this system, ensure entity is remove if exists
            if ((signature & system_signature) == system_signature) {
                system->entities.insert(entity);
            } else {
                system->entities.erase(entity);
            }
        }
    }

    void update_all_render(Coordinator &world, double step_seconds) {
        for (auto &[_, system_list] : systems_by_priority) {
            for (auto &system : system_list) {
                if (system->is_render_system()) {
                    system->update(world, step_seconds);
                }
            }
        }
    }

    void update_all_standard(Coordinator &world, double step_seconds) {
        for (auto &[_, system_list] : systems_by_priority) {
            for (auto &system : system_list) {
                if (!system->is_render_system()) {
                    system->update(world, step_seconds);
                }
            }
        }
    }

private:
    std::unordered_map<std::size_t, Signature> signatures;
    std::unordered_map<std::size_t, std::shared_ptr<System>> systems;
    std::map<int, std::vector<std::shared_ptr<System>>> systems_by_priority;
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_SYSTEM_MANAGER_H_
