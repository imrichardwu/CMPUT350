#include "Tank.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

Tank::Tank(Team team, const Vec2 &pos, AttackPolicy pol, bool bounce_)
    : Unit(team, pos, 20, 80,    // radius, attack_radius
           15, 4.0, 100),        // max_speed, damage, hp
      policy(pol),
      bounce(bounce_) {}

// execute one attack
void Tank::act(World &w) {
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
// collisions[i] = true iff unit hit wall i
// bounce off walls iff bounce is set
void Tank::collision_hook(double prev_speed, bool collisions[4]) {
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
