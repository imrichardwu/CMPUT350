#ifndef TRIANG_H_
#define TRIANG_H_

// triangulation

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <limits>
#include <vector>

// big enough to correctly handle coordinates up to 29600 (see in_circle)
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
// return = 0 if a,b,c are colinear
// return < 0 if a,b,c are ordered in clockwise order
Coordinate orientation_test(const Point &a, const Point &b, const Point &c);

// returns < 0 if d lies inside circle(a,b,c)
// return  = 0 if d on circle(a,b,c)
// return  > 0 otherwise
// assumes a,b,c in clockwise order
// (otherwise result is negated)
Coordinate circle_test(const Point &a, const Point &b, const Point &c, const Point &d);

// a triangle

/*
         p1        pi are points stored in points[]
    n0\  /| n1     ni are indexes of neighboring triangles
       \/ |--->    stored in neighbors[]
       /__|
     p0  | p2      In border triangles index BORDER is used
      n2 v         for outside facing edges
 */

struct Triangle {
    // special triangle index indicating the rectangle border
    // (max size_t value (unsigned!))
    static const std::size_t BORDER = std::numeric_limits<std::size_t>::max();

    // points in clockwise (cw) order
    std::array<Point, 3> points;

    // indexes of neighboring triangles
    // neighbors[i] reached by crossing edge
    // point[i] -> point[(i+1)%3]
    std::array<std::size_t, 3> neighbors;

    // return < 0 if point p lies in the interior
    // return = 0 if point p lies on the perimeter
    // return > 0, otherwise
    // assumes triangle points in clockwise order
    int inside_test(const Point &p) const;

    int inside_test2(const Point &p) const;

    // ensure clockwise order
    void check() const {
        assert(orientation_test(points[0], points[1], points[2]) < 0);
    }

    void print() const {
        for (std::size_t i = 0; i < 3; ++i) {
            points[i].print();
            std::cout << " # " << neighbors[i] << std::endl;
        }
    }
};

class Triangulation {
public:
    // store dimensions and triangulate bounding rectangle
    // (0,0)->(w,0)->(w,h)->(0,h)->(0,0)
    // precond: w, h > 0
    Triangulation(Coordinate w, Coordinate h);

    // add point and repair triangulation
    // precond: p must lie in the interior of the
    // bounding rectangle
    void add_point(const Point &p);

    // return index of a triangle that contains p using an algorithm of your
    // choice
    //
    // precond: p lies inside bounding rectangle or on its perimeter
    std::size_t locate(const Point &p) const;

    // getters
    Coordinate get_width() const {
        return width;
    }
    Coordinate get_height() const {
        return height;
    }
    const std::vector<Triangle> &get_triangles() const {
        return triangles;
    }

    // Check every triangle once by visiting all of its edges and flipping those
    // that increase the minimum interior angle locally.
    // If flag is set, stop after first flip.
    // return number of flipped edges (0 => done)
    std::size_t Delaunay_pass(bool stop_after_first = false);

private:
    // helpers
    void set_neighbor(std::size_t neighbor_ind, std::size_t old_ind, std::size_t new_ind);
    void split_triangle(std::size_t ind, std::size_t new_ind, const Point &p, std::size_t other, std::size_t new_other);

    // consider triangle with index t_ind and edge with index e
    // flip edge iff this increases the minimum interior angle locally
    // return true if edge was flipped, false otherwise
    bool flip_if_better(std::size_t t_ind, std::size_t e);

    // dimension of the bounding rectangle
    Coordinate width, height;

    // all triangles
    std::vector<Triangle> triangles;
};

#endif    // TRIANG_H_
