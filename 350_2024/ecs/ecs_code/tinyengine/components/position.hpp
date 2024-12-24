#ifndef TINYENGINE_COMPONENT_POSITION_H_
#define TINYENGINE_COMPONENT_POSITION_H_

namespace tinyengine::component {

template <typename T>
struct Position {
    T x{};
    T y{};
};

}    // namespace tinyengine::component

#endif    // TINYENGINE_COMPONENT_POSITION_H_
