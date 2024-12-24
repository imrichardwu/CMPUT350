#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Marine.h"
#include "Tank.h"
#include "Unit.h"
#include "World.h"

using namespace std;

constexpr int n_marines = 100;
constexpr int n_tanks = 100;
constexpr bool bounce = true;

class SolWorld : public World {
public:
    SolWorld(double w, double h, int seed) : World(w, h, seed) {}

    void move_unit(Unit &u) {
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

    ~SolWorld() {
        // clean up
        for (Unit *p : units) {
            delete p;
        }
    }

    // deducts hit points and calls attack_hook

    void attack(Unit &attacker, Unit &attacked) {
        attacked.hp -= attacker.damage;
        attack_hook(attacker, attacked);
    }

    // return a random position at which a circle of that radius fits

    Vec2 rnd_pos(double radius) const {
        double slack = radius * 2;

        Vec2 p((width - 2 * slack) * rnd01() + slack, (height - 2 * slack) * rnd01() + slack);

        // assert circle in rectangle
        assert(p.x > radius && p.x < width - radius);
        assert(p.y > radius && p.y < height - radius);
        return p;
    }

    // return uniform random heading
    // length of vector = 1

    Vec2 rnd_heading() const {
        double phi = 2 * M_PI * rnd01();
        return Vec2(cos(phi), sin(phi));
    }

    // mirror position with respect to map mid-point

    Vec2 mirror(const Vec2 &pos) const {
        return Vec2(width - pos.x, height - pos.y);
    }

    // return squared distance between two unit centers

    static double distance2(const Unit &u, const Unit &v) {
        return square(u.pos.x - v.pos.x) + square(u.pos.y - v.pos.y);
    }

    // return true iff u can attack v, i.e. distance of u's and v's centers is
    // less than u's attack distance plus v's radius

    static bool can_attack(const Unit &u, const Unit &v) {
        return distance2(u, v) < square(u.attack_radius + v.radius);
    }

    // populate a vector with enemy units that can be attacked by u;
    // clears vector first
    void enemies_within_attack_range(const Unit &u, vector<Unit *> &targets) const {
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

    Unit *random_weakest_target(Unit &u) const {
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

    Unit *random_closest_target(Unit &u) const {
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

    Unit *random_most_dangerous_target(Unit &u) const {
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

    int red_score() const {
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

    void step_noattack() {
        // remove dead objects
        int killed = 0;
        for (size_t i = 0; i < units.size(); ++i) {
            if (units[i]->hp <= 0) {
                delete units[i];
                units[i] = units[units.size() - 1];
                units.pop_back();
                --i;
                ++killed;
            }
        }
        for (auto p : units) {
            if (p->current_speed > 0) {
                move_unit(*p);
            }
        }
    }
};

constexpr double EPSILON = 1e-6;

bool is_close(double result, double expected, double eps) {
    return std::abs(result - expected) < eps;
}

void test_rnd_heading(bool &ok, World *world, SolWorld *_) {
    std::cout << "Testing rnd_heading ... ";
    for (int i = 0; i < 1000; ++i) {
        Vec2 rnd_heading = world->rnd_heading();
        if (!is_close(std::sqrt(std::pow(rnd_heading.x, 2) + std::pow(rnd_heading.y, 2)), 1.0, EPSILON)) {
            std::cout << "Failed - not unit vector" << std::endl;
            ok &= false;
            return;
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_rnd_pos(bool &ok, World *world, SolWorld *_) {
    std::cout << "Testing rnd_pos ... ";
    for (int r = 1; r < 10; ++r) {
        for (int i = 0; i < 1000; ++i) {
            Vec2 rnd_pos = world->rnd_pos(r);
            if ((rnd_pos.x - r < 0) || (rnd_pos.y - r < 0) || (rnd_pos.x + r >= world->width) ||
                (rnd_pos.y + r >= world->height)) {
                std::cout << "Failed - point outside world box" << std::endl;
                ok &= false;
                return;
            }
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_mirror(bool &ok, World *world, SolWorld *sol_world) {
    std::cout << "Testing mirror ... ";
    for (int r = 1; r < 10; ++r) {
        for (int i = 0; i < 1000; ++i) {
            Vec2 rnd_pos = sol_world->rnd_pos(r);
            Vec2 m_pos = world->mirror(rnd_pos);
            Vec2 sol_pos = sol_world->mirror(rnd_pos);

            if (m_pos.x != sol_pos.x || m_pos.y != sol_pos.y) {
                std::cout << "Failed - Mirrored position doesn't match" << std::endl;
                ok &= false;
                return;
            }
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_distance2(bool &ok, World *world, SolWorld *sol_world) {
    std::cout << "Testing distance2 ... ";
    for (auto const &u1 : sol_world->units) {
        for (auto const &u2 : sol_world->units) {
            double d = World::distance2(*u1, *u2);
            double sol_d = SolWorld::distance2(*u1, *u2);
            if (!is_close(d, sol_d, EPSILON)) {
                std::cout << "Failed - distances2 doesn't match" << std::endl;
                ok &= false;
                return;
            }
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_can_attack(bool &ok, World *world, SolWorld *sol_world) {
    std::cout << "Testing can_attack ... ";

    for (auto const &u1 : sol_world->units) {
        for (auto const &u2 : sol_world->units) {
            bool can_attack = World::can_attack(*u1, *u2);
            bool sol_can_attack = SolWorld::can_attack(*u1, *u2);
            if (can_attack ^ sol_can_attack) {
                std::cout << "Failed - disagreement on whether can attack" << std::endl;
                ok &= false;
                return;
            }
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_enemies_within_attack_range(bool &ok, World *world, SolWorld *sol_world) {
    std::cout << "Testing enemies_within_attack_range ... ";

    for (auto const &u : sol_world->units) {
        vector<Unit *> targets;
        vector<Unit *> sol_targets;
        world->enemies_within_attack_range(*u, targets);
        sol_world->enemies_within_attack_range(*u, sol_targets);
        std::sort(targets.begin(), targets.end());
        std::sort(sol_targets.begin(), sol_targets.end());
        if (targets.size() != sol_targets.size() || targets != sol_targets) {
            std::cout << "Failed - missmatch for enemies" << std::endl;
            ok &= false;
            return;
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_random_weakest_target(bool &ok, World *world, SolWorld *sol_world) {
    std::cout << "Testing random_weakest_target ... ";
    for (auto const &u : sol_world->units) {
        Unit *target = world->random_weakest_target(*u);
        Unit *sol_target = sol_world->random_weakest_target(*u);
        bool is_null = target == 0;
        bool is_null_sol = sol_target == 0;
        if ((is_null ^ is_null_sol) || (!is_null && target->hp_old != sol_target->hp_old)) {
            std::cout << "Failed - missmatch for target" << std::endl;
            ok &= false;
            return;
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_random_closest_target(bool &ok, World *world, SolWorld *sol_world) {
    std::cout << "Testing random_closest_target ... ";
    for (auto const &u : sol_world->units) {
        Unit *target = world->random_closest_target(*u);
        Unit *sol_target = sol_world->random_closest_target(*u);
        bool is_null = target == 0;
        bool is_null_sol = sol_target == 0;
        if ((is_null ^ is_null_sol) || (!is_null && target->hp_old != sol_target->hp_old)) {
            std::cout << "Failed - missmatch for target" << std::endl;
            ok &= false;
            return;
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_random_most_dangerous_target(bool &ok, World *world, SolWorld *sol_world) {
    std::cout << "Testing random_most_dangerous_target ... ";
    for (auto const &u : sol_world->units) {
        Unit *target = world->random_most_dangerous_target(*u);
        Unit *sol_target = sol_world->random_most_dangerous_target(*u);
        bool is_null = target == 0;
        bool is_null_sol = sol_target == 0;
        if ((is_null ^ is_null_sol) || (!is_null && target->hp_old != sol_target->hp_old)) {
            std::cout << "Failed - missmatch for target" << std::endl;
            ok &= false;
            return;
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

void test_move_unit(bool &ok, World *world, SolWorld *sol_world) {
    std::cout << "Testing move_unit ... ";
    for (int step = 0; step < 100; ++step) {
        for (size_t i = 0; i < sol_world->units.size(); ++i) {
            if (sol_world->units[i]->hp <= 0) {
                delete sol_world->units[i];
                sol_world->units[i] = sol_world->units[sol_world->units.size() - 1];
                sol_world->units.pop_back();
                --i;
            }
        }
        for (size_t i = 0; i < world->units.size(); ++i) {
            if (world->units[i]->hp <= 0) {
                delete world->units[i];
                world->units[i] = sol_world->units[world->units.size() - 1];
                world->units.pop_back();
                --i;
            }
        }
        for (auto p : sol_world->units) {
            if (p->current_speed > 0) {
                sol_world->move_unit(*p);
            }
        }
        for (auto p : world->units) {
            if (p->current_speed > 0) {
                world->move_unit(*p);
            }
        }
        // Compare
        if (sol_world->units.size() != world->units.size()) {
            std::cout << "Failed - Number of units doesn't match" << std::endl;
            ok &= false;
            return;
        }
        for (size_t i = 0; i < sol_world->units.size(); ++i) {
            Unit *u1 = sol_world->units[i];
            Unit *u2 = world->units[i];
            if (u1->pos.x != u2->pos.x || u1->pos.y != u2->pos.y) {
                std::cout << "Failed - position doesn't match" << std::endl;
                ok &= false;
                return;
            }
            if (u1->heading.x != u2->heading.x || u1->heading.y != u2->heading.y) {
                std::cout << "Failed - heading doesn't match" << std::endl;
                ok &= false;
                return;
            }
        }
    }
    std::cout << " Passes" << std::endl;
    ok &= true;
}

int main() {
    Marine *m = new Marine(RED, Vec2(0, 0), ATTACK_WEAKEST, false);
    double mr = m->radius;
    delete m;

    Tank *t = new Tank(RED, Vec2(0, 0), ATTACK_WEAKEST, false);
    double tr = t->radius;
    delete t;

    bool ok = true;
    World *world = new World(700, 700, 0);
    SolWorld *sol_world = new SolWorld(700, 700, 0);

    AttackPolicy red_policy = ATTACK_MOST_DANGEROUS;
    AttackPolicy blue_policy = ATTACK_MOST_DANGEROUS;
    AttackPolicy red_marine_policy = red_policy;
    AttackPolicy blue_marine_policy = blue_policy;
    AttackPolicy red_tank_policy = red_policy;
    AttackPolicy blue_tank_policy = blue_policy;

    // create marines
    for (int i = 0; i < n_marines; ++i) {
        Unit *u = new Marine(RED, sol_world->rnd_pos(mr), red_marine_policy, bounce);
        u->heading = sol_world->rnd_heading();
        u->current_speed = u->max_speed;
        sol_world->units.push_back(u);

        // mirrored
        Unit *v = new Marine(BLUE, sol_world->mirror(u->pos), blue_marine_policy, bounce);
        v->heading = Vec2(-u->heading.x, -u->heading.y);
        v->current_speed = v->max_speed;
        sol_world->units.push_back(v);
    }

    // create tanks
    for (int i = 0; i < n_tanks; ++i) {
        Unit *u = new Tank(RED, sol_world->rnd_pos(tr), red_tank_policy, bounce);
        u->heading = sol_world->rnd_heading();
        u->current_speed = u->max_speed;
        sol_world->units.push_back(u);

        // mirrored
        Unit *v = new Tank(BLUE, sol_world->mirror(u->pos), blue_tank_policy, bounce);
        v->heading = Vec2(-u->heading.x, -u->heading.y);
        v->current_speed = v->max_speed;
        sol_world->units.push_back(v);
    }

    world->units = sol_world->units;

    test_rnd_heading(ok, world, sol_world);
    test_rnd_pos(ok, world, sol_world);
    test_mirror(ok, world, sol_world);
    test_distance2(ok, world, sol_world);
    test_can_attack(ok, world, sol_world);
    test_enemies_within_attack_range(ok, world, sol_world);

    // Move the world forward a bit?
    for (int s = 0; s < 10; s++) {
        sol_world->step();
    }
    world->units = sol_world->units;

    test_random_weakest_target(ok, world, sol_world);
    test_random_closest_target(ok, world, sol_world);
    test_random_most_dangerous_target(ok, world, sol_world);

    // Move units test, need to create deep copies of units
    for (int i = 0; i < sol_world->units.size(); ++i) {
        delete sol_world->units[i];
    }
    sol_world->units.clear();
    world->units.clear();
    // create marines
    for (int i = 0; i < n_marines; ++i) {
        auto pos = sol_world->rnd_pos(mr);
        auto heading = sol_world->rnd_heading();
        Unit *u1 = new Marine(RED, pos, red_marine_policy, bounce);
        u1->heading = heading;
        u1->current_speed = u1->max_speed;
        sol_world->units.push_back(u1);
        Unit *u2 = new Marine(RED, pos, red_marine_policy, bounce);
        u2->heading = heading;
        u2->current_speed = u2->max_speed;
        world->units.push_back(u2);

        // mirrored
        Unit *v1 = new Marine(BLUE, sol_world->mirror(u1->pos), blue_marine_policy, bounce);
        v1->heading = Vec2(-u1->heading.x, -u1->heading.y);
        v1->current_speed = v1->max_speed;
        sol_world->units.push_back(v1);
        Unit *v2 = new Marine(BLUE, world->mirror(u2->pos), blue_marine_policy, bounce);
        v2->heading = Vec2(-u2->heading.x, -u2->heading.y);
        v2->current_speed = v2->max_speed;
        world->units.push_back(v2);
    }
    // create tanks
    for (int i = 0; i < n_tanks; ++i) {
        auto pos = sol_world->rnd_pos(tr);
        auto heading = sol_world->rnd_heading();
        Unit *u1 = new Tank(RED, pos, red_tank_policy, bounce);
        u1->heading = heading;
        u1->current_speed = u1->max_speed;
        sol_world->units.push_back(u1);
        Unit *u2 = new Tank(RED, pos, red_tank_policy, bounce);
        u2->heading = heading;
        u2->current_speed = u2->max_speed;
        world->units.push_back(u2);

        // mirrored
        Unit *v1 = new Tank(BLUE, world->mirror(u1->pos), blue_tank_policy, bounce);
        v1->heading = Vec2(-u1->heading.x, -u1->heading.y);
        v1->current_speed = v1->max_speed;
        sol_world->units.push_back(v1);
        Unit *v2 = new Tank(BLUE, world->mirror(u2->pos), blue_tank_policy, bounce);
        v2->heading = Vec2(-u2->heading.x, -u2->heading.y);
        v2->current_speed = v2->max_speed;
        world->units.push_back(v2);
    }
    test_move_unit(ok, world, sol_world);

    // Done
    for (int i = 0; i < sol_world->units.size(); ++i) {
        delete sol_world->units[i];
    }
    for (int i = 0; i < world->units.size(); ++i) {
        delete world->units[i];
    }

    if (ok) {
        std::cout << "all tests passed" << std::endl;
        return 0;
    } else {
        std::cout << "at least one test failed!" << std::endl;
        return 1;
    }
}