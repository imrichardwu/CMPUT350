#include "World.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Unit.h"

using namespace std;

/* helper that you need to implement

 - move unit by current_speed * heading

 - units only collide with the map border

 - when units hit a wall they stop at the collision point (current_speed=0)
   and collision_hook is called with the previous speed and a vector of bools
   encoding the walls that were hit. This vector is used in collision_hook to
   implement bouncing by only changing the unit's heading, i.e. even when
   bouncing the unit stays at the collision location for the remainder of the
   simulation frame and only starts moving again in the following frame.

 - it is essential that units are located inside the map at all times. There
   can be no overlap. Ensure this property by clipping coordinates after
   moving.

*/

void World::move_unit(Unit &u) {
    double dx = u.heading.x * u.current_speed;
    double dy = u.heading.y * u.current_speed;
    const double big_t = 2;
    double t = big_t;
    double new_x = u.pos.x + dx;
    double new_y = u.pos.y + dy;
    bool collision[4] = {false, false, false, false};

    // cout << u.pos.x << " " << u.pos.y << " - " << new_x << " " << new_y;

    if (dx < 0 && new_x - u.radius < 0) {
        // cout << " LEFT " << new_x - u.radius << endl;
        collision[LEFT] = true;
        t = std::min(t, 1.0 - (new_x - u.radius) / dx);
    }
    if (dx > 0 && new_x + u.radius > width) {
        // cout << " RIGHT " << new_x + u.radius - width << endl;
        collision[RIGHT] = true;
        t = std::min(t, 1.0 - (new_x + u.radius - width) / dx);
    }
    if (dy < 0 && new_y - u.radius < 0) {
        // cout << " TOP " << new_y - u.radius << endl;
        collision[TOP] = true;
        t = std::min(t, 1.0 - (new_y - u.radius) / dy);
    }
    if (dy > 0 && new_y + u.radius > height) {
        // cout << " BOTTOM " << new_y + u.radius - height << endl;
        collision[BOTTOM] = true;
        t = std::min(t, 1.0 - (new_y + u.radius - height) / dy);
    }

    if (t >= big_t) {
        t = 1;
    }

    // cout << " t=" << t;

    u.pos.x += dx * t;
    u.pos.y += dy * t;

    // clip to box
    if (u.pos.x - u.radius < 0) u.pos.x = u.radius;
    if (u.pos.x + u.radius > width) u.pos.x = width - u.radius;
    if (u.pos.y - u.radius < 0) u.pos.y = u.radius;
    if (u.pos.y + u.radius > height) u.pos.y = height - u.radius;

    if (t < 1) {
        // cout << " stop at " << u.pos.x << " " << u.pos.y << endl;
        double s = u.current_speed;
        // stop unit
        u.current_speed = 0;
        // inform unit that it has collided
        u.collision_hook(s, collision);
    }
    // cout << endl;
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
    // clean up
    for (Unit *p : units) {
        delete p;
    }
}

// deducts hit points and calls attack_hook

void World::attack(Unit &attacker, Unit &attacked) {
    attacked.hp -= attacker.damage;
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
    double phi = 2 * M_PI * rnd01();
    return Vec2(cos(phi), sin(phi));
}

// mirror position with respect to map mid-point

Vec2 World::mirror(const Vec2 &pos) const {
    return Vec2(width - pos.x, height - pos.y);
}

// return squared distance between two unit centers

double World::distance2(const Unit &u, const Unit &v) {
    return square(u.pos.x - v.pos.x) + square(u.pos.y - v.pos.y);
}

// return true iff u can attack v, i.e. distance of u's and v's centers is
// less than u's attack distance plus v's radius

bool World::can_attack(const Unit &u, const Unit &v) {
    return distance2(u, v) < square(u.attack_radius + v.radius);
}

// populate a vector with enemy units that can be attacked by u;
// clears vector first
void World::enemies_within_attack_range(const Unit &u, vector<Unit *> &targets) const {
    targets.clear();

    for (auto &p : units) {
        if (u.team == p->team) {
            continue;    // same team
        }
        if (can_attack(u, *p)) {
            targets.push_back(p);
        }
    }
}

// return a random unit that can be attacked by u with minimal hp_old value,
// or 0 if none exists

Unit *World::random_weakest_target(Unit &u) const {
    vector<Unit *> targets;
    enemies_within_attack_range(u, targets);

    size_t n = targets.size();

    if (!n) {
        return 0;
    }

    int min = targets[0]->hp_old;
    vector<Unit *> optimal;
    optimal.push_back(targets[0]);

    assert(can_attack(u, *targets[0]));

    for (size_t i = 1; i < n; ++i) {
        assert(can_attack(u, *targets[i]));

        int hp = targets[i]->hp_old;
        if (hp > min) {
            continue;
        }
        if (hp < min) {
            min = hp;
            optimal.clear();
        }
        optimal.push_back(targets[i]);
    }
    // choose a random optimal target
    assert(optimal.size());
    return optimal[rnd_int(optimal.size())];
}

// return a random unit that can be attacked by u with minimal distance to u,
// or 0 if none exists

Unit *World::random_closest_target(Unit &u) const {
    // attack closest

    vector<Unit *> targets;
    enemies_within_attack_range(u, targets);

    size_t n = targets.size();

    if (!n) {
        return 0;
    }

    double min = distance2(u, *targets[0]);
    vector<Unit *> optimal;

    optimal.push_back(targets[0]);
    assert(can_attack(u, *targets[0]));

    for (size_t i = 1; i < n; ++i) {
        assert(can_attack(u, *targets[i]));

        double d = distance2(u, *targets[i]);
        if (d > min) continue;
        if (d < min) {
            min = d;
            optimal.clear();
        }
        optimal.push_back(targets[i]);
    }

    // choose a random optimal target
    assert(optimal.size());
    return optimal[rnd_int(optimal.size())];
}

// return a random unit that can be attacked by u with maximal damage/hp_old
// ratio, or 0 if none exists

Unit *World::random_most_dangerous_target(Unit &u) const {
    vector<Unit *> targets;
    enemies_within_attack_range(u, targets);

    size_t n = targets.size();

    if (!n) {
        return 0;
    }

    double max = targets[0]->damage / targets[0]->hp_old;
    vector<Unit *> optimal;

    optimal.push_back(targets[0]);
    assert(can_attack(u, *targets[0]));

    for (size_t i = 1; i < n; ++i) {
        assert(can_attack(u, *targets[i]));

        double p = targets[i]->damage / targets[i]->hp_old;
        if (p < max) {
            continue;
        }
        if (p > max) {
            max = p;
            optimal.clear();
        }
        optimal.push_back(targets[i]);
    }

    // choose a random optimal target
    assert(optimal.size());
    return optimal[rnd_int(optimal.size())];
}

// return score for red: 2 for win, 0 for loss, 1 for draw, -1 for game not
// over yet

int World::red_score() const {
    if (units.empty()) {
        return 1;
    }

    size_t red_units = 0;
    for (Unit *p : units) {
        red_units += p->team == RED;
    }

    if (red_units == units.size()) {
        return 2;
    }

    if (!red_units) {
        return 0;
    }

    return -1;
}
