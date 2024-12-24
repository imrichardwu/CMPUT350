#ifndef TRIANG_H_
#define TRIANG_H_

// triangulation

#include <cstddef>
#include <iostream>
#include <vector>

// big enough to correctly handle coordinates up to 29'600 (see circle_test)
using Coordinate = long long;

// a point on the integer grid

struct Point {
    Point(Coordinate x_ = 0, Coordinate y_ = 0)
        : x(x_), y(y_) {}

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }
    bool operator!=(const Point &p) const {
        return x != p.x || y != p.y;
    }

    void print() const {
        std::cout << x << " " << y;
    }

    Coordinate x, y;
};

// for points a,b,c
// return > 0 if a,b,c are ordered in counter-clockwise order
// return = 0 if a,b,c are collinear
// return < 0 if a,b,c are ordered in clockwise order
Coordinate orientation_test(const Point &a, const Point &b, const Point &c);

// returns < 0 if d lies inside circle(a,b,c)
// return  = 0 if d on circle(a,b,c)
// return  > 0 otherwise
// assumes a,b,c in clockwise order
// (otherwise result is negated)
Coordinate circle_test(const Point &a, const Point &b, const Point &c, const Point &d);

// for axis-aligned rectangle r given by diagonally opposing points p1, p2
// return < 0 if p lies inside r
// return = 0 if p lies on r's border
// return > 0 if p lies outside r
Coordinate rectangle_test(const Point &p1, const Point &p2, const Point &p);

#endif    // TRIANG_H_
