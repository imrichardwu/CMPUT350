#ifndef TINYENGINE_COMPONENTS_VELOCITY_H_
#define TINYENGINE_COMPONENTS_VELOCITY_H_

#include "tinyengine/core/vec2.hpp"

namespace tinyengine::component {

struct Velocity2 {
    Vec2 heading;
    float speed{};
};

}    // namespace tinyengine::component

#endif    // TINYENGINE_COMPONENTS_VELOCITY_H_
