#include "World.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Unit.h"

using namespace std;

/* helper that you need to implement

 - move unit by (current_speed * heading)

 - units only collide with the map border

 - when a unit hits a wall it stops at the collision point (current_speed=0)
   and collision_hook needs to be called with the previous speed and an array
   of bools encoding all the walls that were hit (the array indexes need to be
   consistent with enum CollDir). This array is used to implement bouncing by
   only changing the unit's heading, i.e. even when bouncing the unit stays at
   the collision location for the remainder of the simulation frame and only
   starts moving again in the following frame

 - it is essential that units are located inside the map at all times. There
   can be no overlap. Ensure this property by clipping coordinates after
   moving

*/

void World::move_unit(Unit &u) {

    // Move unit
    u.pos.x += u.current_speed * u.heading.x;
    u.pos.y += u.current_speed * u.heading.y;

    // Check for collisions
    bool collisions[4] = {false, false, false, false};

    if (u.pos.x - u.radius < 0) {
        // Collision with left wall
        u.pos.x = u.radius;
        collisions[LEFT] = true;
    } else if (u.pos.x + u.radius > width) {
        // Collision with right wall
        u.pos.x = width - u.radius;
        collisions[RIGHT] = true;
    }

    if (u.pos.y - u.radius < 0) {
        // Collision with top wall
        u.pos.y = u.radius;
        collisions[TOP] = true;
    } else if (u.pos.y + u.radius > height) {
        // Collision with bottom wall
        u.pos.y = height - u.radius;
        collisions[BOTTOM] = true;
    }

    // Call collision hook
    if (collisions[LEFT] || collisions[RIGHT] || collisions[TOP] || collisions[BOTTOM]) {
        u.collision_hook(u.current_speed, collisions);
    }

}

// returns policy name
const char *apol2str(AttackPolicy pol) {
    switch (pol) {
        case ATTACK_WEAKEST:
            return "attack-weakest";
        case ATTACK_CLOSEST:
            return "attack-closest";
        case ATTACK_MOST_DANGEROUS:
            return "attack-most-dangerous";
    }
    return "?";
}

World::~World() {
    // delete all units
    for(Unit *u : units) {
        delete u;
    }
}

// deducts hit points and calls attack_hook

void World::attack(Unit &attacker, Unit &attacked) {
    // Deduct hit points from the attacked unit
    attacked.hp -= attacker.damage; 

    // Call attack_hook to inform about the attack
    attack_hook(attacker, attacked);
}

// return a random position at which a circle of that radius fits

Vec2 World::rnd_pos(double radius) const {
    double slack = radius * 2;

    Vec2 p((width - 2 * slack) * rnd01() + slack, (height - 2 * slack) * rnd01() + slack);

    // assert circle in rectangle
    assert(p.x > radius && p.x < width - radius);
    assert(p.y > radius && p.y < height - radius);
    return p;
}

// return uniform random heading
// length of vector = 1

Vec2 World::rnd_heading() const {
    double angle = rnd01() * 2 * M_PI;  // Random angle between 0 and 2π
    return Vec2(cos(angle), sin(angle));
}

// mirror position with respect to map mid-point

Vec2 World::mirror(const Vec2 &pos) const {
    // Calculate mid-point of the map
    Vec2 mid_point(width / 2.0, height / 2.0);

    // Calculate mirrored position
    Vec2 mirrored_pos(2 * mid_point.x - pos.x, 2 * mid_point.y - pos.y);

    // assert mirrored position in map
    return mirrored_pos;
}

// return squared distance between two unit centers
double World::distance2(const Unit &u, const Unit &v) {
    return square(u.pos.x - v.pos.x) + square(u.pos.y - v.pos.y);
}

// return true iff u can attack v, i.e. distance of u's and v's centers is
// less than u's attack distance plus v's radius
bool World::can_attack(const Unit &u, const Unit &v) {
    if (u.team == v.team) {
        return false;
    }

    return distance2(u, v) <= square(u.attack_radius + v.radius);
}

// populate a vector with enemy units that can be attacked by u;
// clears vector first
void World::enemies_within_attack_range(const Unit &u, vector<Unit *> &targets) const {
    targets.clear();

    // Iterate through all units
    for (Unit *v : units) {
        // Check if v is an enemy unit that can be attacked by u
        if (can_attack(u, *v)) {
            // Add v to the list of targets
            targets.push_back(v);
        }
    }
}

// return a random unit that can be attacked by u with minimal hp_old value,
// or 0 if none exists

Unit *World::random_weakest_target(Unit &u) const {
    vector<Unit *> targets;

    enemies_within_attack_range(u, targets);

    if (targets.empty()) {
        return nullptr;
    }

    // Find the minimal hp_old value among targets
    int min_hp = std::numeric_limits<int>::max();
    for (Unit *t : targets) {
        if (t->hp_old < min_hp) {
            min_hp = t->hp_old;
        }
    }

    // Collect all targets with minimal hp_old
    vector<Unit *> weakest_targets;
    for (Unit *t : targets) {
        if (t->hp_old == min_hp) {
            weakest_targets.push_back(t);
        }
    }

    // Return a random target among the weakest
    int index = rnd_int(static_cast<int>(weakest_targets.size()));
    return weakest_targets[index];
}

// return a random unit that can be attacked by u with minimal distance to
// u, or 0 if none exists

Unit *World::random_closest_target(Unit &u) const {
    vector<Unit *> targets;

    enemies_within_attack_range(u, targets);

    if (targets.empty()) {
        return nullptr;
    }

    // Find the minimal distance to u among targets
    double min_distance = std::numeric_limits<double>::max();
    for (Unit *t : targets) {
        double distance = distance2(u, *t);
        if (distance < min_distance) {
            min_distance = distance;
        }
    }

    // Collect all targets with minimal distance
    vector<Unit *> closest_targets;
    for (Unit *t : targets) {
        if (distance2(u, *t) == min_distance) {
            closest_targets.push_back(t);
        }
    }

    // Return a random target among the closest
    int index = rnd_int(static_cast<int>(closest_targets.size()));
    return closest_targets[index];
}

// return a random unit that can be attacked by u with maximal damage/hp_old
// ratio, or 0 if none exists

Unit *World::random_most_dangerous_target(Unit &u) const {
    vector<Unit *> targets;

    enemies_within_attack_range(u, targets);

     if (targets.empty()) {
        return nullptr;
    }

    // Find the maximal damage/hp_old ratio among targets
    double max_ratio = -1.0;
    for (Unit *t : targets) {
        if (t->hp_old > 0) {  // Avoid division by zero
            double ratio = t->damage / static_cast<double>(t->hp_old);
            if (ratio > max_ratio) {
                max_ratio = ratio;
            }
        }
    }

    // Collect all targets with maximal ratio
    vector<Unit *> dangerous_targets;
    for (Unit *t : targets) {
        if (t->hp_old > 0) {
            double ratio = t->damage / static_cast<double>(t->hp_old);
            if (ratio == max_ratio) {
                dangerous_targets.push_back(t);
            }
        }
    }

    // Return a random target among the most dangerous
    int index = rnd_int(static_cast<int>(dangerous_targets.size()));
    return dangerous_targets[index];
}

// return score for red: 2 for win, 0 for loss, 1 for draw, -1 for game not
// over yet

int World::red_score() const {
    if (units.empty()) {
        return 1;
    }

    bool red_alive = false;
    bool blue_alive = false;

    for (Unit *u : units) {
        if (u->team == RED) {
            red_alive = true;
        } else {
            blue_alive = true;
        }
    }

   if (red_alive && !blue_alive) {
        return 2;  // Red wins
    } else if (!red_alive && blue_alive) {
        return 0;  // Blue wins
    } else if (!red_alive && !blue_alive) {
        return 1;  // Draw
    } else {
        return -1;  // Game not over yet
    }
}
