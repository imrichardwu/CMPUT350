#ifndef TINYENGINE_CORE_ENTITY_MANAGER_H_
#define TINYENGINE_CORE_ENTITY_MANAGER_H_

#include "tinyengine/core/types.hpp"

#include <array>
#include <queue>

namespace tinyengine {

class EntityManager {
public:
    EntityManager();

    // Take ID from front of the queue if we have not handed out all our entities
    auto create_entity() -> Entity;

    // Take back the entity to give out later again
    void destroy_entity(Entity entity);

    void set_entity_signature(Entity entity, Signature signature);

    auto get_entity_signature(Entity entity) -> Signature;

private:
    std::queue<Entity> available_entities;
    std::array<Signature, MAX_ENTITIES> entity_signatures;
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_ENTITY_MANAGER_H_
