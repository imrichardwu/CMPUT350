#include "tinyengine/core/entity_manager.h"

#include "tinyengine/core/exception.h"
#include "tinyengine/core/types.hpp"

#include <array>
#include <cassert>
#include <queue>

namespace tinyengine {

EntityManager::EntityManager() {
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        available_entities.push(entity);
    }
}

// Take ID from front of the queue if we have not handed out all our entities
auto EntityManager::create_entity() -> Entity {
    if (available_entities.empty()) {
        TE_EXCEPTION("No more entities to give out.");
    }

    Entity entity_id = available_entities.front();
    available_entities.pop();
    return entity_id;
}

// Take back the entity to give out later again
void EntityManager::destroy_entity(Entity entity) {
    assert(entity < MAX_ENTITIES);
    entity_signatures[entity].reset();    // NOLINT(*-array-index)
    available_entities.push(entity);
}

void EntityManager::set_entity_signature(Entity entity, Signature signature) {
    assert(entity < MAX_ENTITIES);
    entity_signatures[entity] = std::move(signature);    // NOLINT(*-array-index)
}

auto EntityManager::get_entity_signature(Entity entity) -> Signature {
    assert(entity < MAX_ENTITIES);
    return entity_signatures[entity];    // NOLINT(*-array-index)
}

}    // namespace tinyengine
