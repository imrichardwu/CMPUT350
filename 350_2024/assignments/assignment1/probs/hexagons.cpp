/* Assignment 1, Part 4

Hexagons!

You are excited that the map representation of the new game you will be
working on is based on the following hexagonal tessellation:

                   ...
      y             ___
      ^  ...     3 /   \      ...
      |        ___/  .  \___
 ...  |     2 /   \ 3 3 /   \ (u,v)              hexagon size/indexes:
      |   ___/  .  \___/  .  \_|_                     ___
      |1 /   \ 2 2 /   \ 4 3 / * \   ...             /\  \
     _|_/  .  \___/  .  \___/  .  \                 / r\  \
j:0 / | \ 1 1 /   \ 3 2 /   \ 5 3 /                 \ i j /
   /  .__\___/__.__\___/__.__\___/______> x          \___/
   \ 0 0 /   \ 2 1 /   \ 4 2 /   \
    \___/  .  \___/  .  \___/  .  \
        \ 1 0 /   \ 3 1 /   \ 5 2 /   ...
   ...   \___/     \___/     \___/

  i:  0    1    2    3    4    5   ...

To start with your pathfinding implementation, you decide to first write C++
function

  void locate(double r, double x, double y, int &i, int &j)

that - given hexagon radius r > 0 and a 2d point (x,y) - computes integer
coordinates (i,j) of *a* hexagon whose center point is closest to (x,y). In
case of ambiguities you are free to output any valid (i,j) you'd like.

For example, for point (u,v), marked with * in above diagram, the result would
be (5,3).

The (x=0,y=0) origin is the center point of hexagon (i=0,j=0), and the hexagon
locations and orientations are as depicted above. Note that x and y are
arbitrary floating point numbers (thus, i or j could be negative, for
instance), but you can assume that the function results will not exceed the
out-parameters' value range. Also, for simplicity, we will accept code that is
based on floating point computations, which are prone to rounding errors, and
therefore can lead to wrong results in rare cases. However, when testing your
function we'll try to stay clear of such scenarios, and you can assume that
your floating point based implementation (using C type double) just works.

Here are a few valid input/output examples:

  r       x      y   :  i   j

  1       0      0      0   0
  1       3      0      2   1
  0.97    6.6  -15.4    4  -7
  6.96 -108.4  185.6  -10  10


In the sections marked implement... below, implement and test your function

Also, at the end of this file, explain in a few paragraphs how your locate
function and your testing method works in detail


Plan of attack:

- The central question is this: how does (i,j) relate to (x,y)?

  To answer this, use above tessellation diagram to figure out how to compute
  the center point (x,y) of a hexagon given its integer coordinates (i,j) and
  then invert the process, i.e. computing (i,j) from center point
  (x,y). Pythagoras may have something to say about that ...

- Using above derivation you will be able to compute tight lower bounds i_ and
  j_ for i and j, respectively, by rounding values down to the next
  integer. Tight, here, means that i_+1 and j_+1 will be upper bounds for i
  and j, respectively

  With this insight, you will be able to check a small constant number of
  cases to determine (i,j)

  Note, that your code MUST be based on above idea

- Test your code extensively. You may find it useful to use the functions we
  provide for this task below, e.g. for generating millions of random test
  samples, similar to how we will test your implementation...

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
 *   /\  \
 *  / 1\  \  center at (x=0,y=0)
 *  \     /
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

// computes indexes (i,j) of a hexagon whose center point is closest to (x,y)

void locate(double r, double x, double y, int &i, int &j) {
    assert(r > 0);

    // implement...
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

    cout << "all tests passed" << endl;
    return 0;
}

/*

  Explain how your locate function and your testing method work in detail
  here:




*/
