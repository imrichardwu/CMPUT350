#include "Marine.h"

// ... implement

Marine::Marine(Team team, const Vec2 &pos, AttackPolicy pol, bool bounce)
    : Unit(team, pos, 1, 5, 1, 1, 10), policy(pol), bounce(bounce) {
        // Initialize base Unit with Marine stats
    }

void Marine::act(World &w) {
    Unit *target = nullptr;

    // Choose target depending on policy
   if (policy == ATTACK_WEAKEST) {
        target = w.random_weakest_target(*this);
    } else if (policy == ATTACK_CLOSEST) {
        target = w.random_closest_target(*this);
    } else if (policy == ATTACK_MOST_DANGEROUS) {
        target = w.random_most_dangerous_target(*this);
    }

    // If there is a target, attack it
    if (target) {
        w.attack(*this, *target);
    }
}

void Marine::collision_hook(double prev_speed, bool collisions[4]) {
    if (bounce) {
        // Bounce off walls
        if (collisions[RIGHT] || collisions[LEFT]) {
            heading.x *= -1;
        }
        if (collisions[TOP] || collisions[BOTTOM]) {
            heading.y *= -1;
        }

        // Normalize the heading vector
        double length = sqrt(heading.x * heading.x + heading.y * heading.y);
        if (length != 0) {
            heading.x /= length;
            heading.y /= length;
        }

        // Reduce speed using energy loss factor
        const double ENERGY_LOSS_FACTOR = 0.95;  // Retain 95% of the previous speed
        current_speed = prev_speed * ENERGY_LOSS_FACTOR;

        // Ensure current_speed does not exceed max_speed
        if (current_speed > max_speed) {
            current_speed = max_speed;
        }

        // Stop the unit if speed falls below minimum threshold
        const double MIN_SPEED = 0.2;
        if (current_speed < MIN_SPEED) {
            current_speed = 0.0;
        }

    } else {
        // Stop when hitting a wall
        current_speed = 0.0;
    }
}

