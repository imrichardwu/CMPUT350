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

    // Compute floating-point i and j
    double iFloat = x / (1.5 * r);
    double jFloat = y / (sqrt(3) * r) + 0.5 * iFloat;

    // Compute floor values
    int iFloor = floor(iFloat);
    int jFloor = floor(jFloat);

    // Initialize minimum distance squared to a large number
    double minDistanceSquared = 1e20;
    int optimalI = iFloor;
    int optimalJ = jFloor;

    // Iterate over the four possible candidates
    for(int deltaI = 0; deltaI <= 1; deltaI++) {
        for(int deltaJ = 0; deltaJ <= 1; deltaJ++) {
            int candidateI = iFloor + deltaI;
            int candidateJ = jFloor + deltaJ;

            // Compute center of the candidate hexagon
            double candidateCenterX = 1.5 * r * candidateI;
            double candidateCenterY = sqrt(3) * r * (candidateJ - 0.5 * candidateI);

            // Compute squared distance to the point (x, y)
            double distanceSquared = sq(candidateCenterX - x) + sq(candidateCenterY - y);

            // Update the optimal (i, j) if this candidate is closer
            if(distanceSquared < minDistanceSquared) {
                minDistanceSquared = distanceSquared;
                optimalI = candidateI;
                optimalJ = candidateJ;
            }
        }
    }

    // Set the output parameters
    i = optimalI;
    j = optimalJ;
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
/*
 * Function: runAdditionalTests
 * --------------------------------------------
 *   Performs extensive testing by generating random points within a range of hexagons
 *   and verifying that the `locate` function correctly identifies their corresponding
 *   hexagon indices.
 *
 *   This function leverages the `test` function to validate each generated point.
 */
void runAdditionalTests() {
    // Configuration for additional tests
    const int testsPerHexagon = 1000; // Number of random tests per hexagon
    const int rangeI = 20;             // Range of i indices to test (from -20 to 20)
    const int rangeJ = 20;             // Range of j indices to test (from -20 to 20)
    const double hexRadius = 1.0;      // Radius of each hexagon

    // Iterate over the specified range of i and j indices
    for(int currentI = -rangeI; currentI <= rangeI; currentI++) {
        for(int currentJ = -rangeJ; currentJ <= rangeJ; currentJ++) {
            // Calculate the center coordinates of the current hexagon
            double centerX = 1.5 * hexRadius * currentI;
            double centerY = sqrt(3) * hexRadius * (currentJ - 0.5 * currentI);

            // Perform the specified number of random tests for each hexagon
            for(int testNumber = 0; testNumber < testsPerHexagon; testNumber++) {
                double relativeX, relativeY;

                // Generate a random point within the unit hexagon
                random_point_inside_flat_top_unit_hexagon(relativeX, relativeY);

                // Scale and translate the point to the current hexagon's position
                double testX = centerX + relativeX * hexRadius;
                double testY = centerY + relativeY * hexRadius;

                // Use the test function to validate the locate function
                test(hexRadius, testX, testY, currentI, currentJ);
            }
        }
    }
}

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

    runAdditionalTests();

    cout << "all tests passed" << endl;
    return 0;
}

/*

    Explain how your locate function and your testing method work in detail
    here:

The `locate` function is designed to determine the integer coordinates `(i, j)` of the hexagon whose center is closest to a given point `(x, y)` on a 2D plane, based on a hexagonal tessellation with a specified radius `r`.

**1. Coordinate System Mapping:**
    - In a flat-top hexagonal grid, each hexagon center can be mapped to `(x, y)` coordinates using the following relationships:
        - **X-coordinate:** `x = 1.5 * r * i`
        - **Y-coordinate:** `y = sqrt(3) * r * (j - 0.5 * i)`
    - These equations account for the horizontal spacing (`1.5 * r`) between hexagon centers and the vertical spacing (`sqrt(3) * r`) adjusted by the `i` index.

**2. Inversion to Find `(i, j)`:**
    - To find `(i, j)` from a given `(x, y)`, the function inverts the above equations:
        - Compute the floating-point `iFloat = x / (1.5 * r)`
        - Compute the floating-point `jFloat = (y / (sqrt(3) * r)) + 0.5 * iFloat`
    - These floating-point values represent the theoretical indices which are then floored to obtain integer bounds:
        - `iFloor = floor(iFloat)`
        - `jFloor = floor(jFloat)`

**3. Candidate Selection:**
    - Since the actual `(i, j)` could be one of the neighboring cells around `(iFloor, jFloor)`, the function iterates over the four possible candidates:
        - `(iFloor, jFloor)`
        - `(iFloor + 1, jFloor)`
        - `(iFloor, jFloor + 1)`
        - `(iFloor + 1, jFloor + 1)`.
    - For each candidate, it calculates the center `(candidateCenterX, candidateCenterY)`using the original mapping equations and computes the squared Euclidean distance to the point `(x, y)`.

**4. Selection of the Closest Hexagon:**
    - The candidate with the minimum distance squared is selected as the closest hexagon, and its `(i, j)` indices are returned.

**Testing Method Explanation:**

**1. Randomized Testing:**
    - The `runAdditionalTests` function performs extensive randomized testing to cover a wide range of scenarios and edge cases.
    - **Test Generation:**
        - The testing spans a grid of hexagons defined by a range of `i` and `j` indices (e.g., from `-20` to `20` for both axes).
        - For each hexagon within this range, a specified number of random points (`testsPerHexagon`, e.g., 1000) are generated inside the hexagon using the `random_point_inside_flat_top_unit_hexagon` function. This ensures that points are uniformly distributed within each hexagon.
        - These points are then scaled and translated to their actual positions based on the current hexagon's center coordinates.
    - **Validation:**
        - For each generated point `(testX, testY)`, the `locate` function is invoked to compute the corresponding `(i, j)` indices.
        - The `test` function then verifies that the returned indices match the expected `(currentI, currentJ)` of the hexagon in which the point was generated.
        - If a mismatch is detected, the test outputs an error message and terminates, indicating a failure in the `locate` function.
    - **Coverage:**
        - By covering a broad range of hexagon indices and generating numerous random points within each, the testing method ensures that the `locate` function is rigorously evaluated across diverse positions, including those near hexagon boundaries where ambiguities might arise.

*/