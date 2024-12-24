// test hexagons solution

// includes hexagons.cpp and tests it
// uses XXX_ prefix for test-local variables and functions

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

// rename their main and include file
#define main YYY_main
#include "hexagons.cpp"
#undef main

using namespace std;

// helper: return square

double XXX_sq(double x) {
    return x * x;
}

// helper functions you may find useful for testing your locate function

// vvv============================================================================

std::random_device XXX_r;
std::mt19937 XXX_rng(XXX_r());    // Mersenne Twister RNG, random seed

/* return true iff (x,y) lies inside a regular unit hexagon (r=1, center
 * (0,0)) with flat top:
 *
 *    ___
 *   /\  \
 *  / 1\  \
 *  \     /
 *   \___/
 *
 * based on:
 * https://stackoverflow.com/questions/5193331/is-a-point-inside-regular-hexagon/5195868#5195868
 * Note that this function must not be used by locate() itself
 */

bool XXX_inside_flat_top_unit_hexagon(double x, double y) {
    // check length (squared) against inner and outer radius
    double l2 = XXX_sq(x) + XXX_sq(y);
    if (l2 > 1.0) {
        return false;
    }
    if (l2 < 0.75) {
        return true;
    }    // (sqrt(3)/2)^2 = 3/4

    swap(x, y);    // flat top (code was for corner top)

    // transform and check against borders
    double px = x * 2.0 / sqrt(3);
    if (px > 1 || px < -1) {
        return false;
    }

    double py = 0.5 * px + y;
    if (py > 1 || py < -1) {
        return false;
    }

    return !((px - py > 1 || px - py < -1));
}

/* compute uniformly distributed point (x,y) in regular flat top unit hexagon
 * centered at (0,0)
 */

void XXX_random_point_inside_flat_top_unit_hexagon(double &x, double &y) {
    uniform_real_distribution<double> dist(-1.0, 1.0);

    // rejection sampling

    for (;;) {
        x = dist(XXX_rng);
        y = dist(XXX_rng);
        if (XXX_inside_flat_top_unit_hexagon(x, y)) {
            break;
        }
    }
}

// ^^^============================================================================

// functions for generating test samples

void XXX_center(double r, int i, int j, double &cx, double &cy) {
    cx = i * 1.5 * r;
    cy = (-i + 2 * j) * 0.5 * sqrt(3) * r;
}

void XXX_generate_test_sample(double r, double &x, double &y, int &i, int &j) {
    // random point inside radius r hexagon centered at (0,0)

    XXX_random_point_inside_flat_top_unit_hexagon(x, y);
    x *= r;
    y *= r;

    // random hexagon

    uniform_int_distribution<int> dist(-10, 10);

    i = dist(XXX_rng);
    j = dist(XXX_rng);

    double cx, cy;
    XXX_center(r, i, j, cx, cy);

    // random point inside random hexagon
    x += cx;
    y += cy;
}

// checks locate(r, x, y, ...) results by comparing them with known unique
// solution (i,j)

void XXX_test(double r, double x, double y, int i, int j) {
    int i2, j2;

    locate(r, x, y, i2, j2);

    if (i != i2 || j != j2) {
        cout << r << " " << x << " " << y << " "
             << " : " << i2 << " " << j2;
        cout << " oops! should be " << i << " " << j << endl;
        exit(1);
    }
}

int main() {
    // test with random points

    for (int k = 0; k < 1'000'000; ++k) {
        uniform_real_distribution<double> dist(0.1, 10);

        double r = dist(XXX_rng);
        double x, y;
        int i, j;

        XXX_generate_test_sample(r, x, y, i, j);
        // cout << r << " " << x << " " << y << " : " << i << " " << j << endl;
        XXX_test(r, x, y, i, j);
    }

    return 0;
}