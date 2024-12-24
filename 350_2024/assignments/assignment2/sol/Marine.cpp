#include "Marine.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

Marine::Marine(Team team, const Vec2 &pos, AttackPolicy pol, bool bounce_)
    : Unit(team, pos, 10, 40,    // radius, attack_radius
           10, 1.0, 45),         // max_speed, damage, hp
      policy(pol),
      bounce(bounce_) {}

// execute one attack
void Marine::act(World &w) {
    Unit *target = 0;

    switch (policy) {
        case ATTACK_WEAKEST:
            target = w.random_weakest_target(*this);
            break;

        case ATTACK_CLOSEST:
            target = w.random_closest_target(*this);
            break;

        case ATTACK_MOST_DANGEROUS:
            target = w.random_most_dangerous_target(*this);
            break;
    }

    if (target) {
        w.attack(*this, *target);
    }
}

// react to collision
// collisions[i] = true iff unit hit wall i
// bounce off walls iff bounce is set
void Marine::collision_hook(double prev_speed, bool collisions[4]) {
    if (!bounce) return;

    if (collisions[RIGHT] || collisions[LEFT]) {
        current_speed = prev_speed;
        heading.x = -heading.x;
    }
    if (collisions[TOP] || collisions[BOTTOM]) {
        current_speed = prev_speed;
        heading.y = -heading.y;
    }
}
