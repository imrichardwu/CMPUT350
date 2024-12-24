#ifndef TINYENGINE_CORE_TYPES_H_
#define TINYENGINE_CORE_TYPES_H_

#include <bitset>
#include <cstddef>

namespace tinyengine {

// ECS
using Entity = std::size_t;
constexpr Entity MAX_ENTITIES = 5000;
using ComponentType = std::size_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_TYPES_H_
