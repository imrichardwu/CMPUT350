/* Assignment 1, Part 3

Hexagons!

You are excited that the map representation of the new game you will be
working on is based on the following hexagonal tessellation:

                   ...
 y                 ___
 ^              3 /   \      ...
 |  ...       ___/  .  \___
 |         2 /   \ 3 3 /   \                        (x,y)
         ___/  .  \___/  .  \___                     _|_
      1 /   \ 2 2 /   \ 4 3 /   \                   / * \
    ___/  .  \___/  .  \___/  .  \                 /  .  \
 0 /   \ 1 1 /   \ 3 2 /   \ 5 3 /                 \ i j /
  /  .  \___/  .  \___/  .  \___/   ...             \___/
  \ 0 0 /   \ 2 1 /   \ 4 2 /   \
   \___/  .  \___/  .  \___/  .  \
       \ 1 0 /   \ 3 1 /   \ 5 2 /
        \___/     \___/     \___/
                                         ----> x
     0    1    2    3    4    5   ...

To start with your pathfinding implementation you decide to first write C++
function

  void locate(double r, double x, double y, int &i, int &j)

that - given hexagon radius r > 0 and a 2d point (x,y) - computes integer
coordinates (i,j) of a hexagon whose center point is closest to (x,y). The
(x=0,y=0) origin is the center point of hexagon (i=0,j=0), and the hexagon
locations and orientations are as depicted above. Note that x and y are
arbitrary floating point numbers (thus, i or j could be negative, for
instance), but you can assume that the function results will not exceed the
out-parameters' value range. Also, for simplicity, we will accept code that is
based on floating point computations, which are prone to rounding errors, and
therefore can lead to wrong results in rare cases. However, when testing your
function we'll try to stay clear of such scenarios.

Here are a few valid input/output examples:

  r       x      y   :  i   j

  1       0      0      0   0
  1       3      0      2   1
  0.97    6.6  -15.4    4  -7
  6.96 -108.4  185.6  -10  10

In the sections marked implement... implement and test your algorithm


Plan of attack:

- The central question is this: how does (i,j) relate to (x,y)?

  To answer this, use above tessellation diagram to figure out how to compute
  the center point (x,y) of a hexagon given its integer coordinates (i,j) and
  then invert the process. Pythagoras may have something to say about that ...

- From the derivation above you will then be able to compute tight lower
  bounds i_ and j_ for i and j, respectively, by rounding down. Tight means
  that i_+1 and j_+1 will be upper bounds for i and j, respectively

  With this insight, you will be able to check a small constant number of
  cases to determine (i,j)

  Note, that your code MUST be based on above idea

- Test your code extensively. You may find it useful to use the functions we
  provide for this task below, e.g. for generating millions of random test
  samples, similar to how we will check your implementation...

*/

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

using namespace std;

// helper: return square

double sq(double x) {
    return x * x;
}

// helper functions you may find useful for testing your locate function

// vvv============================================================================

std::random_device r;
std::mt19937 rng(r());    // Mersenne Twister RNG, random seed

/* return true iff (x,y) lies inside a regular unit hexagon (r=1, center
 * (0,0)) with flat top:
 *
 *    ___
 *   /   \
 *  /  ._ \
 *  \   1 /
 *   \___/
 *
 * based on:
 * https://stackoverflow.com/questions/5193331/is-a-point-inside-regular-hexagon/5195868#5195868
 * Note that this function must not be used by locate() itself
 */

bool inside_flat_top_unit_hexagon(double x, double y) {
    // check length (squared) against inner and outer radius
    double l2 = sq(x) + sq(y);
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

void random_point_inside_flat_top_unit_hexagon(double &x, double &y) {
    uniform_real_distribution<double> dist(-1.0, 1.0);

    // rejection sampling

    for (;;) {
        x = dist(rng);
        y = dist(rng);
        if (inside_flat_top_unit_hexagon(x, y)) {
            break;
        }
    }
}

// ^^^============================================================================

// functions for generating test samples

void center(double r, int i, int j, double &cx, double &cy) {
    cx = i * 1.5 * r;
    cy = (-i + 2 * j) * 0.5 * sqrt(3) * r;
}

void generate_test_sample(double r, double &x, double &y, int &i, int &j) {
    // random point inside radius r hexagon centered at (0,0)

    random_point_inside_flat_top_unit_hexagon(x, y);
    x *= r;
    y *= r;

    // random hexagon

    uniform_int_distribution<int> dist(-10, 10);

    i = dist(rng);
    j = dist(rng);

    double cx, cy;
    center(r, i, j, cx, cy);

    // random point inside random hexagon
    x += cx;
    y += cy;
}

// computes indexes (i,j) of a hexagon whose center point is closest to (x,y)

void locate(double r, double x, double y, int &i, int &j) {
    assert(r > 0);

    // implement...

    // tight index lower bounds

    int i_ = static_cast<int>(floor(x / (1.5 * r)));
    int j_ = static_cast<int>(floor(x / (3.0 * r) + y / (sqrt(3) * r)));

    // check four neighboring hexagons by increasing indexes by 0 or 1

    double bdd = -1;
    i = j = 0;

    for (int k = 0; k < 4; ++k) {
        int ti = i_ + ((k & 1) != 0);    // lowest order bit
        int tj = j_ + ((k & 2) != 0);    // second lowest order bit

        double cx, cy;
        center(r, ti, tj, cx, cy);

        double dd = sq(cx - x) + sq(cy - y);

        if (bdd < 0 || dd < bdd) {
            // found first or closer hexagon
            bdd = dd;
            i = ti;
            j = tj;
        }
    }
}

// checks locate(r, x, y, ...) results by comparing them with known unique
// solution (i,j)

void test(double r, double x, double y, int i, int j) {
    int i2, j2;

    locate(r, x, y, i2, j2);

    if (i != i2 || j != j2) {
        cout << r << " " << x << " " << y << " "
             << " : " << i2 << " " << j2;
        cout << " oops! should be " << i << " " << j << endl;
        exit(1);
    }
}

// test code

int main() {
    test(1, 0, 0, 0, 0);
    test(1, 3, 0, 2, 1);
    test(1, 6, 0, 4, 2);

    test(2, 0, 0, 0, 0);
    test(2, 6, 0, 2, 1);
    test(2, 12, 0, 4, 2);

    test(0.97, 6.6, -15.4, 4, -7);
    test(6.96, -108.4, 185.6, -10, 10);

    // implement...
    // add more test code here
    // ...

    // random points

    for (int k = 0; k < 1'000'000; ++k) {
        uniform_real_distribution<double> dist(0.1, 10);

        double r = dist(rng);
        double x, y;
        int i, j;

        generate_test_sample(r, x, y, i, j);
        // cout << r << " " << x << " " << y << " : " << i << " " << j << endl;
        test(r, x, y, i, j);
    }

    cout << "all tests passed" << endl;
    return 0;
}
