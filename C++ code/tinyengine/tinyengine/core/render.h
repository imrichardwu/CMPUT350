#ifndef TINYENGINE_CORE_RENDER_H_
#define TINYENGINE_CORE_RENDER_H_

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <cstdint>
#include <string>

namespace tinyengine {

struct Color {
    uint8_t r{};
    uint8_t g{};
    uint8_t b{};
    uint8_t a = 255;
};

constexpr Color kBlack{0, 0, 0};
constexpr Color kWhite{255, 255, 255};
constexpr Color kRed{255, 0, 0};
constexpr Color kBlue{0, 255, 0};
constexpr Color kGreen{0, 0, 255};

class Render {
public:
    ~Render();

    static void init(int window_width, int window_height, const std::string &window_title);

    static auto get_instance() -> Render &;

    // Can move but not copy
    Render(const Render &) noexcept = delete;
    Render(Render &&) noexcept = delete;
    Render &operator=(const Render &) noexcept = delete;
    Render &operator=(Render &&) noexcept = delete;

    [[nodiscard]] inline auto get_width() const -> int {
        return window_width;
    }
    [[nodiscard]] inline auto get_height() const -> int {
        return window_height;
    }

    void clear();
    void present();

    void draw_text(
        int x,
        int y,
        int font_size,
        const Color &text_color,
        const std::string &text,
        bool center_text = false
    );

    void draw_circle(int x, int y, int radius, const Color &color, bool fill = false);

private:
    Render(int window_width, int window_height, const std::string &window_title);

    [[nodiscard]] auto get_current_color() const -> Color;
    void set_current_color(const Color &color);

    static inline int window_width = 0;
    static inline int window_height = 0;
    static inline std::string window_title = "";
    static inline bool initialized = false;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_RWops *font_data = nullptr;
};

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_RENDER_H_
