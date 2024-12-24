#include "Triang.h"

#include <algorithm>
#include <cassert>

using namespace std;

// prep problem 2

// for axis-aligned rectangle r given by diagonally opposing points p1, p2
// return < 0 if p lies inside r
// return = 0 if p lies on r's border
// return > 0 if p lies outside r
Coordinate rectangle_test(const Point &p1, const Point &p2, const Point &p) {
    Coordinate xmin = min(p1.x, p2.x);
    Coordinate xmax = max(p1.x, p2.x);
    Coordinate ymin = min(p1.y, p2.y);
    Coordinate ymax = max(p1.y, p2.y);

    if (p.x < xmin || p.x > xmax || p.y < ymin || p.y > ymax) {
        // outside
        return +1;
    }

    if (p.x > xmin && p.x < xmax && p.y > ymin && p.y < ymax) {
        // inside
        return -1;
    }

    // border
    return 0;
}

int main() {
    // prep problem 1
    {
        // test orientation()

        Point A(0, 0);
        Point B(5, 5);
        Point C(5, 0);

        assert(orientation_test(A, B, C) < 0);
        assert(orientation_test(A, C, B) > 0);

        Point D(6, 6);
        assert(orientation_test(A, B, D) == 0);
    }

    {
        // test in_circle()
        Point A(0, 10);
        Point B(10, 0);
        Point C(0, -10);
        Point D(-10, 0);    // on circle
        Point E(0, 0);      // in circle
        Point F(20, 20);    // outside circle

        assert(orientation_test(A, B, C) < 0);

        assert(circle_test(A, B, C, D) == 0);
        assert(circle_test(A, B, C, E) < 0);
        assert(circle_test(A, B, C, F) > 0);
    }

    // prep problem 2
    {
        Point p1(0, 0);
        Point p2(10, 10);
        assert(rectangle_test(p1, p2, {1, 1}) < 0);
        assert(rectangle_test(p1, p2, {5, 5}) < 0);
        assert(rectangle_test(p1, p2, {9, 9}) < 0);
        assert(rectangle_test(p1, p2, {0, 5}) == 0);
        assert(rectangle_test(p1, p2, {10, 5}) == 0);
        assert(rectangle_test(p1, p2, {5, 0}) == 0);
        assert(rectangle_test(p1, p2, {5, 10}) == 0);
        assert(rectangle_test(p1, p2, {5, 15}) > 0);
        assert(rectangle_test(p1, p2, {5, -5}) > 0);
        assert(rectangle_test(p1, p2, {15, 5}) > 0);
        assert(rectangle_test(p1, p2, {-5, 5}) > 0);
    }
    return 0;
}
