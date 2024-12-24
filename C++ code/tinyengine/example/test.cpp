#include <tinyengine/components/heading.hpp>
#include <tinyengine/components/position.hpp>
#include <tinyengine/components/radius.hpp>
#include <tinyengine/components/speed.hpp>
#include <tinyengine/core/coordinator.h>
#include <tinyengine/core/engine.h>
#include <tinyengine/core/render.h>
#include <tinyengine/core/system.h>

#include <array>
#include <type_traits>

namespace {
template <typename E>
constexpr auto to_underlying(E e) noexcept -> std::underlying_type_t<E> {
    return static_cast<std::underlying_type_t<E>>(e);
}
}    // namespace

// Components
using PositionComponent = tinyengine::component::Position<float>;
using HeadingComponent = tinyengine::component::Heading<float>;
using SpeedComponent = tinyengine::component::Speed<float>;
using RadiusComponent = tinyengine::component::Radius<float>;

struct CollisionComponent {
    enum class Direction : std::size_t {
        RIGHT = 0,
        LEFT,
        TOP,
        BOTTOM
    };
    auto operator[](Direction dir) const -> bool {
        return collisions[to_underlying(dir)];    // NOLINT(*-constant-array-index)
    }
    auto operator[](Direction dir) -> bool & {
        return collisions[to_underlying(dir)];    // NOLINT(*-constant-array-index)
    }
    void reset() {
        for (auto &c : collisions) {
            c = false;
        }
    }
    std::array<bool, 4> collisions{};
};

// Systems
class MovementSystem : public tinyengine::System {
public:
    void update(tinyengine::Coordinator &world, double step_seconds) override {
        [[maybe_unused]] const auto &width = world.get_data<int>("width");
        [[maybe_unused]] const auto &height = world.get_data<int>("height");
        std::array<double, 2> ratio_travelled = {1, 1};
        double ratio = 2;

        for (const auto &entity : entities) {
            auto &position = world.get_component<PositionComponent>(entity);
            auto &collision = world.get_component<CollisionComponent>(entity);

            const auto &heading = world.get_component<HeadingComponent>(entity);
            const auto &speed = world.get_component<SpeedComponent>(entity);
            const auto &r = world.get_component<RadiusComponent>(entity).val;
            const auto &width = world.get_data<int>("width");
            const auto &height = world.get_data<int>("height");

            const auto dx = heading.x * speed.val * step_seconds;
            const auto dy = heading.y * speed.val * step_seconds;

            const auto new_x = position.x + dx;
            const auto new_y = position.y + dy;

            // Check each bounding box direction
            if (dx < 0 && new_x - r < 0) {
                collision[CollisionComponent::Direction::LEFT] = true;
                ratio_travelled[0] = 1.0 - (new_x - r) / dx;
                ratio = std::min(ratio, ratio_travelled[0]);
            } else if (dx > 0 && new_x + r > width) {
                collision[CollisionComponent::Direction::RIGHT] = true;
                ratio_travelled[0] = 1.0 - (new_x + r - width) / dx;
                ratio = std::min(ratio, ratio_travelled[0]);
            }
            if (dy < 0 && new_y - r < 0) {
                collision[CollisionComponent::Direction::TOP] = true;
                ratio_travelled[1] = 1.0 - (new_y - r) / dy;
                ratio = std::min(ratio, ratio_travelled[1]);
            } else if (dy > 0 && new_y + r > height) {
                collision[CollisionComponent::Direction::BOTTOM] = true;
                ratio_travelled[1] = 1.0 - (new_y + r - height) / dy;
                ratio = std::min(ratio, ratio_travelled[1]);
            }
            auto t = std::min(1.0, std::min(ratio_travelled[0], ratio_travelled[1]));
            auto _t = std::min(1.0, ratio);
            t = _t;

            position.x += static_cast<decltype(position.x)>(dx * t);
            position.y += static_cast<decltype(position.y)>(dy * t);
        }
    }
};

class RenderSystem : public tinyengine::System {
public:
    RenderSystem()
        : System(true) {}
    void update(tinyengine::Coordinator &world, [[maybe_unused]] double step_seconds) override {
        for (const auto &entity : entities) {
            const auto &position = world.get_component<PositionComponent>(entity);
            const auto &radius = world.get_component<RadiusComponent>(entity);
            const auto x = static_cast<int>(position.x);
            const auto y = static_cast<int>(position.y);
            const auto r = static_cast<int>(radius.val);
            tinyengine::Render::get_instance().draw_circle(x, y, r, tinyengine::kRed);
        }
    }
};

class CollisionSystem : public tinyengine::System {
public:
    void update(tinyengine::Coordinator &world, [[maybe_unused]] double step_seconds) override {
        for (const auto &entity : entities) {
            auto &collisions = world.get_component<CollisionComponent>(entity);
            auto &heading = world.get_component<HeadingComponent>(entity);
            if (collisions[CollisionComponent::Direction::LEFT] || collisions[CollisionComponent::Direction::RIGHT]) {
                heading.x = -heading.x;
            }
            if (collisions[CollisionComponent::Direction::TOP] || collisions[CollisionComponent::Direction::BOTTOM]) {
                heading.y = -heading.y;
            }
            collisions.reset();
        }
    }
};

int main() {
    tinyengine::Coordinator world;

    // Register components
    world.register_component<PositionComponent>();
    world.register_component<HeadingComponent>();
    world.register_component<SpeedComponent>();
    world.register_component<CollisionComponent>();
    world.register_component<RadiusComponent>();

    // Register systems
    {
        auto movement_system = world.register_system<MovementSystem>(1);
        tinyengine::Signature signature;
        signature.set(world.get_component_type<PositionComponent>());
        signature.set(world.get_component_type<HeadingComponent>());
        signature.set(world.get_component_type<SpeedComponent>());
        signature.set(world.get_component_type<CollisionComponent>());
        signature.set(world.get_component_type<RadiusComponent>());
        world.set_system_signature<MovementSystem>(signature);
    }
    {
        auto render_system = world.register_system<RenderSystem>(2);
        tinyengine::Signature signature;
        signature.set(world.get_component_type<PositionComponent>());
        signature.set(world.get_component_type<RadiusComponent>());
        world.set_system_signature<RenderSystem>(signature);
    }
    {
        auto collision_system = world.register_system<CollisionSystem>(1);
        tinyengine::Signature signature;
        signature.set(world.get_component_type<CollisionComponent>());
        signature.set(world.get_component_type<HeadingComponent>());
        world.set_system_signature<CollisionSystem>(signature);
    }

    // Entities
    {
        auto entity = world.create_entity();
        world.add_component(entity, PositionComponent{150, 210});
        world.add_component(entity, HeadingComponent{-0.8, -1});
        world.add_component(entity, SpeedComponent{50});
        world.add_component(entity, RadiusComponent{20});
        world.add_component(entity, CollisionComponent{});
    }

    constexpr int width = 800;
    constexpr int height = 800;
    constexpr int tick_rate = 60;

    // Store data
    world.store_data("width", width);
    world.store_data("height", height);

    tinyengine::Engine engine(tick_rate);
    engine.set_video(width, height, "test");
    engine.run(world);
}
