#ifndef QUAD_TREE_TYPES_H_
#define QUAD_TREE_TYPES_H_

#include "constants.h"

#include <array>
#include <exception>

namespace quadtree {

enum class Quadrant {
    NORTH_WEST = 0,
    NORTH_EAST = 1,
    SOUTH_WEST = 2,
    SOUTH_EAST = 3,
    NONE = 4
};

struct Point {
    enum CollDir {
        RIGHT = 0,
        LEFT,
        TOP,
        BOTTOM
    };

    auto operator==(const Point &rhs) -> bool {
        return x == rhs.x && y == rhs.y && heading_x == rhs.heading_x && heading_y == rhs.heading_y;
    }

    void move() {
        double dx = heading_x * SPEED;
        double dy = heading_y * SPEED;
        const double big_t = 2;
        double t = big_t;
        double new_x = x + dx;
        double new_y = y + dy;
        std::array<bool, 4> collision = {false, false, false, false};

        if (dx < 0 && new_x - RADIUS < 0) {
            collision[LEFT] = true;
            t = std::min(t, 1.0 - (new_x - RADIUS) / dx);
        }
        if (dx > 0 && new_x + RADIUS > WIDTH) {
            collision[RIGHT] = true;
            t = std::min(t, 1.0 - (new_x + RADIUS - WIDTH) / dx);
        }
        if (dy < 0 && new_y - RADIUS < 0) {
            collision[TOP] = true;
            t = std::min(t, 1.0 - (new_y - RADIUS) / dy);
        }
        if (dy > 0 && new_y + RADIUS > HEIGHT) {
            collision[BOTTOM] = true;
            t = std::min(t, 1.0 - (new_y + RADIUS - HEIGHT) / dy);
        }

        if (t >= big_t) {
            t = 1;
        }

        // cout << " t=" << t;

        x += static_cast<int>(dx * t);
        y += static_cast<int>(dy * t);

        // clip to box
        if (x - RADIUS < 0) {
            x = RADIUS;
        }
        if (x + RADIUS > WIDTH) {
            x = WIDTH - RADIUS;
        }
        if (y - RADIUS < 0) {
            y = RADIUS;
        }
        if (y + RADIUS > HEIGHT) {
            y = HEIGHT - RADIUS;
        }

        if (t < 1) {
            if (collision[RIGHT] || collision[LEFT]) {
                heading_x = -heading_x;
            }
            if (collision[TOP] || collision[BOTTOM]) {
                heading_y = -heading_y;
            }
        }
    }

    int x{};
    int y{};
    double heading_x{};
    double heading_y{};
    float r{};
    float g{};
    float b{};
};
// auto operator==(const Point &lhs, const Point &rhs) -> bool {
//     return lhs.x == rhs.x && lhs.y == rhs.y && lhs.heading_x == rhs.heading_x && lhs.heading_y == rhs.heading_y;
// }

struct BoundingBox {
    [[nodiscard]] constexpr auto right() const -> int {
        return mid_x + half_width;
    }
    [[nodiscard]] constexpr auto left() const -> int {
        return mid_x - half_width;
    }
    [[nodiscard]] constexpr auto top() const -> int {
        return mid_y - half_height;
    }
    [[nodiscard]] constexpr auto bottom() const -> int {
        return mid_y + half_height;
    }
    [[nodiscard]] constexpr auto get_quadrant(const Quadrant &quadrant) const -> BoundingBox {
        switch (quadrant) {
        case Quadrant::NORTH_WEST:
            return {
                .mid_x = mid_x - half_width / 2,
                .mid_y = mid_y - half_height / 2,
                .half_width = half_width / 2,
                .half_height = half_height / 2
            };
        case Quadrant::NORTH_EAST:
            return {
                .mid_x = mid_x + half_width / 2,
                .mid_y = mid_y - half_height / 2,
                .half_width = half_width / 2,
                .half_height = half_height / 2
            };
        case Quadrant::SOUTH_WEST:
            return {
                .mid_x = mid_x - half_width / 2,
                .mid_y = mid_y + half_height / 2,
                .half_width = half_width / 2,
                .half_height = half_height / 2
            };
        case Quadrant::SOUTH_EAST:
            return {
                .mid_x = mid_x + half_width / 2,
                .mid_y = mid_y + half_height / 2,
                .half_width = half_width / 2,
                .half_height = half_height / 2
            };
        default:
            throw std::exception();
        }
    }
    int mid_x;
    int mid_y;
    int half_width;
    int half_height;
};

[[nodiscard]] constexpr auto point_to_bbox(const Point &p) -> BoundingBox {
    return {.mid_x = p.x, .mid_y = p.y, .half_width = RADIUS / 2, .half_height = RADIUS / 2};
}

[[nodiscard]] constexpr auto do_boxes_overlap(const BoundingBox &lhs, const BoundingBox &rhs) -> bool {
    return !(
        lhs.right() < rhs.left() || lhs.left() > rhs.right() || lhs.bottom() < rhs.top() || lhs.top() > rhs.bottom()
    );
}

}    // namespace quadtree

#endif    // QUAD_TREE_TYPES_H_
