#ifndef TINYENGINE_CORE_VEC2_H_
#define TINYENGINE_CORE_VEC2_H_

namespace tinyengine {

struct Vec2 {
    Vec2() = default;
    Vec2(float x, float y)
        : x(x), y(y) {}

    float x{};
    float y{};
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_VEC2_H_
