#ifndef TINYENGINE_COMPONENTS_HEALTH_H_
#define TINYENGINE_COMPONENTS_HEALTH_H_

namespace tinyengine::component {

template <typename T>
struct Health {
    T health{};
};

}    // namespace tinyengine::component

#endif    // TINYENGINE_COMPONENTS_HEALTH_H_
