#include "tinyengine/core/render.h"

#include "tinyengine/core/exception.h"
#include "tinyengine/graphics/sans.bin"

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

#include <cassert>
#include <iostream>

namespace tinyengine {

void Render::init(int width, int height, const std::string &title) {
    window_width = width;
    window_height = height;
    window_title = title;
    initialized = true;
}

auto Render::get_instance() -> Render & {
    // Check if we have previously initialized
    if (!initialized) {
        const std::string error_msg = "Must initialize before getting.";
        std::cerr << error_msg << std::endl;
        TE_EXCEPTION(error_msg);
    }
    static Render render(window_width, window_height, window_title);
    return render;
}

Render::Render(int window_width, int window_height, const std::string &window_title) {
    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0 || TTF_Init() < 0) {
        const std::string error_msg = "Failed to initialize SDL " + std::string(SDL_GetError());
        std::cerr << error_msg << std::endl;
        TE_EXCEPTION(error_msg);
    }

    // Initialize window
    if (SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_OPENGL, &window, &renderer) < 0) {
        const std::string error_msg = "Failed to create window " + std::string(SDL_GetError());
        std::cerr << error_msg << std::endl;
        TE_EXCEPTION(error_msg);
    }

    // Load font binary data
    font_data = SDL_RWFromConstMem(sans_ttf.data(), static_cast<int>(sans_ttf.size()));
    if (!font_data) {
        TE_EXCEPTION("Error loading font.");
    }

    assert(window);
    assert(renderer);
    SDL_SetWindowTitle(window, window_title.c_str());
}

Render::~Render() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeRW(font_data);
    renderer = nullptr;
    window = nullptr;
    font_data = nullptr;
    TTF_Quit();
    SDL_Quit();
}

void Render::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Render::present() {
    SDL_RenderPresent(renderer);
}

void Render::draw_text(int x, int y, int font_size, const Color &color, const std::string &text, bool center_text) {
    if (text == "") {
        return;
    }
    auto font = TTF_OpenFontRW(font_data, 0, font_size);
    if (!font) {
        TE_EXCEPTION("Failed to load font from binary");
    }
    // Load text to surface, then onto texture
    SDL_Surface *surface_text = TTF_RenderText_Solid(font, text.c_str(), {color.r, color.g, color.b});
    SDL_Texture *texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);
    // Draw texture onto rectangle
    auto start_x = center_text ? x - (surface_text->w / 2) : x;
    auto start_y = center_text ? y - (surface_text->h / 2) : y;
    SDL_Rect text_dest = {start_x, start_y, surface_text->w, surface_text->h};
    SDL_RenderCopy(renderer, texture_text, nullptr, &text_dest);
    // Cleanup
    SDL_DestroyTexture(texture_text);
    SDL_FreeSurface(surface_text);
}

auto Render::get_current_color() const -> Color {
    Color color;
    SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a);
    return color;
}

void Render::set_current_color(const Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Render::draw_circle(int center_x, int center_y, int radius, const Color &color, bool fill) {
    Color prev_color = get_current_color();
    set_current_color(color);

    // Midpoint circle algorithm
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }

    // Draw circle
    if (fill) {
    }

    set_current_color(prev_color);
}

}    // namespace tinyengine
