#ifndef TINYENGINE_CORE_ENGINE_H_
#define TINYENGINE_CORE_ENGINE_H_

#include "tinyengine/core/coordinator.h"

#include <chrono>
#include <string>
#include <type_traits>

// Remove this when we remove on_event
#include <SDL2/SDL.h>

namespace tinyengine {

class Engine {
    using TickT = std::remove_reference_t<decltype(std::declval<std::chrono::microseconds::rep>())>;
    constexpr static TickT US_PER_S = 1000000;

public:
    Engine(TickT tick_rate)
        : us_per_update(US_PER_S / tick_rate){};
    virtual ~Engine() = default;

    // Doesn't make sense to copy/move
    Engine(const Engine &) noexcept = delete;
    Engine(Engine &&) noexcept = default;
    Engine &operator=(const Engine &) noexcept = delete;
    Engine &operator=(Engine &&) noexcept = default;

    [[nodiscard]] static auto init() -> Engine &;

    void set_video(int window_width, int window_height, const std::string &window_title);
    void run(Coordinator &world);

protected:
    virtual void on_event(const SDL_Event &event) {};

private:
    // void core_render();
    void core_event();

    TickT us_per_update;    // Engine tick rate in microseconds
    bool quit = false;
    bool is_render = false;
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_ENGINE_H_
