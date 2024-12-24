#include "tinyengine/core/engine.h"

#include "tinyengine/core/coordinator.h"
#include "tinyengine/core/render.h"

#include <chrono>
#include <iostream>
#include <string>

namespace tinyengine {

void Engine::set_video(int window_width, int window_height, const std::string &window_title) {
    // Check if we already have a renderer
    if (is_render) {
        const std::string error_msg = "Attempting to initalize a renderer again.";
        std::cerr << error_msg << std::endl;
        throw std::runtime_error(error_msg);
    }
    Render::init(window_width, window_height, window_title);
    is_render = true;
}

void Engine::run(Coordinator &world) {
    auto time_prev = std::chrono::system_clock::now();
    TickT lag_us = 0;

    const auto step_seconds = static_cast<double>(us_per_update) / US_PER_S;

    while (!quit) {
        // Find time since last game loop
        auto time_curr = std::chrono::system_clock::now();
        auto time_elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(time_curr - time_prev).count();
        time_prev = time_curr;
        lag_us += time_elapsed_us;

        core_event();

        if (is_render) {
            Render::get_instance().clear();
        }

        while (lag_us >= us_per_update) {
            world.update_all_standard_systems(step_seconds);
            lag_us -= us_per_update;
        }

        if (is_render) {
            world.update_all_render_systems(step_seconds);
            Render::get_instance().present();
        }
    }
}

// void Engine::run(Coordinator &world) {
//     std::chrono::time_point<std::chrono::system_clock> time_start, time_end;
//     int x = 200;
//     int y = 200;
//
//     while (!quit) {
//         time_start = std::chrono::system_clock::now();
//
//         core_event();
//
//         if (render) {
//             render->clear();
//         }
//
//         world.update_all_systems();
//
//         if (render) {
//             render->draw_circle(x, y, 20, kRed);
//             render->draw_text(x, y, 24, kWhite, "Jake", true);
//         }
//
//         if (render) {
//             render->present();
//         }
//
//         --x;
//         --y;
//
//         time_end = std::chrono::system_clock::now();
//         auto time_delta_us = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();
//         auto time_to_sleep = delay_per_frame - time_delta_us;
//         if (time_to_sleep > 0) {
//             std::this_thread::sleep_for(std::chrono::microseconds(time_to_sleep));
//         }
//     }
// }

void Engine::core_event() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Core events to check for
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        }

        // User defined event checker
        on_event(event);
    }
}

}    // namespace tinyengine
