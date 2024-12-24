#ifndef TINYENGINE_CORE_SYSTEM_H_
#define TINYENGINE_CORE_SYSTEM_H_

#include "tinyengine/core/types.hpp"

#include <set>

namespace tinyengine {

// Forward declare to avoid recursive includes
class Coordinator;

// Iterated over a collection of entities of a particular signature
// Meant to be subclassed
class System {
public:
    System(bool is_render = false)
        : is_render(is_render) {}
    virtual ~System() = default;

    System(const System &) noexcept = default;
    System(System &&) noexcept = default;
    System &operator=(const System &) noexcept = default;
    System &operator=(System &&) noexcept = default;

    virtual void update([[maybe_unused]] Coordinator &world, [[maybe_unused]] double step_seconds) {}

    virtual auto is_render_system() -> bool {
        return is_render;
    }

    std::set<Entity> entities;
    bool is_render;
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_SYSTEM_H_
