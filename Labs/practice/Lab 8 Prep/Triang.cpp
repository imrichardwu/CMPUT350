#include "Triang.h"

#include <cassert>
#include <cmath>

using namespace std;

/*
  for points a,b,c
    return > 0 if a,b,c are ordered in counter-clockwise order
    return = 0 if a,b,c are collinear
   return < 0 if a,b,c are ordered in clockwise order
*/
Coordinate orientation_test(const Point &a, const Point &b, const Point &c) {
    // computes the z-component of the cross-product of b-a and c-a
    Coordinate bax, bay, cax, cay;
    bax = b.x - a.x;
    bay = b.y - a.y;
    cay = c.y - a.y;
    cax = c.x - a.x;

    // ensure computation below doesn't overflow
    const int N = 1'000'000'000;    // for long long (2*N*N * 2*N*N * 3 < max(Coordinate))
    assert(abs(bax) < N);
    assert(abs(bay) < N);
    assert(abs(cax) < N);
    assert(abs(cay) < N);

    return bax * cay - cax * bay;
}

/*
  adapted from http://www.cs.cmu.edu/~quake/robust.html
    returns < 0 if d lies inside circle(a,b,c)
    return  = 0 if d on circle(a,b,c)
    return  > 0 otherwise
  assumes a,b,c not collinear
  assumes a,b,c in clockwise order
  (otherwise result is negated)
*/
Coordinate circle_test(const Point &a, const Point &b, const Point &c, const Point &d) {
    Coordinate adx, ady, bdx, bdy, cdx, cdy;
    Coordinate abdet, bcdet, cadet;
    Coordinate alift, blift, clift;

    adx = a.x - d.x;
    ady = a.y - d.y;
    bdx = b.x - d.x;
    bdy = b.y - d.y;
    cdx = c.x - d.x;
    cdy = c.y - d.y;

    // ensure computation below doesn't overflow
    const int N = 29'600;    // for long long (2*N*N * 2*N*N * 3 < max(Coordinate))
    assert(abs(adx) < N);
    assert(abs(ady) < N);
    assert(abs(bdx) < N);
    assert(abs(bdy) < N);
    assert(abs(cdx) < N);
    assert(abs(cdy) < N);
    
    assert(orientation_test(a, b, c) != 0);

    abdet = adx * bdy - bdx * ady;
    bcdet = bdx * cdy - cdx * bdy;
    cadet = cdx * ady - adx * cdy;
    alift = adx * adx + ady * ady;
    blift = bdx * bdx + bdy * bdy;
    clift = cdx * cdx + cdy * cdy;

    return alift * bcdet + blift * cadet + clift * abdet;
}

Coordinate rectangle_test(const Point &p1, const Point &p2, const Point &p) {
    // Determine the minimum and maximum x and y coordinates
    Coordinate min_x = std::min(p1.x, p2.x);
    Coordinate max_x = std::max(p1.x, p2.x);
    Coordinate min_y = std::min(p1.y, p2.y);
    Coordinate max_y = std::max(p1.y, p2.y);

    // Check the position of point p relative to the rectangle
    bool inside_x = (p.x > min_x) && (p.x < max_x);
    bool inside_y = (p.y > min_y) && (p.y < max_y);
    bool on_border_x = (p.x == min_x) || (p.x == max_x);
    bool on_border_y = (p.y == min_y) || (p.y == max_y);

    if (inside_x && inside_y) {
        return -1; // Inside the rectangle
    } else if ((inside_x && on_border_y) || (inside_y && on_border_x)) {
        return 0; // On the border of the rectangle
    } else {
        return 1; // Outside the rectangle
    }
}
