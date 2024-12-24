#include "Triang.h"

#include <cassert>
#include <cstddef>
#include <iostream>

//  return < 0 if point p lies in the interior
//  return = 0 if point p lies on the perimeter
//  return > 0, otherwise
//  assumes triangle points in clockwise order
//
//  hint: use orientation() 3 times and distinguish
//        cases given by the number of results != 0
//
int Triangle::inside_test(const Point &p) const {
    // ... implemented

    // follow edges and determine orientation when including p
    Coordinate s1 = orientation_test(points[0], points[1], p);
    Coordinate s2 = orientation_test(points[1], points[2], p);
    Coordinate s3 = orientation_test(points[2], points[0], p);

    // any ccw => outside
    if (s1 > 0 || s2 > 0 || s3 > 0) {
        return +1;
    }

    //  any collinear => on perimeter
    if (s1 == 0 || s2 == 0 || s3 == 0) {
        return 0;
    }

    // inside
    return -1;
}

// store dimensions and triangulate bounding rectangle
// (0,0)->(w,0)->(w,h)->(0,h)->(0,0)
// use BORDER as index of out-of-bounds neighbors (see Triangle Mesh
// Representation on lab page)
// precond: w, h > 0
//
//  Hint: when done the triangulation must contain two triangles which together
// represent the rectangle.  Both triangles must be oriented clockwise - which
// can be checked with Triangle::check()
//
//         BORDER
//   (0,h)  _|_ (w,h)
//         |  /|
// BORDER<-| x |->BORDER
//         |/__|
//   (0,0)   |  (w,0)
//         BORDER
//
Triangulation::Triangulation(Coordinate w, Coordinate h)
    : width(w), height(h) {
    //  ... implemented

    assert(w > 0 && h > 0);

    Triangle a, b;
    a.points[0] = Point(0, 0);
    a.points[1] = Point(0, h);
    a.points[2] = Point(w, h);
    a.neighbors[0] = Triangle::BORDER;
    a.neighbors[1] = Triangle::BORDER;
    a.neighbors[2] = 1;

    b.points[0] = Point(0, 0);
    b.points[1] = Point(w, h);
    b.points[2] = Point(w, 0);
    b.neighbors[0] = 0;
    b.neighbors[1] = Triangle::BORDER;
    b.neighbors[2] = Triangle::BORDER;

    triangles.push_back(a);
    triangles.push_back(b);

    a.check();
    b.check();
}

// Check every triangle once by visiting all of its edges and flipping those
// that increase the minimum interior angle locally.
// If flag is set, stop after first flip.
// return number of flipped edges (0 => done)
//
//  Hint: use flip_if_better()
std::size_t Triangulation::Delaunay_pass(bool stop_after_first) {
    //  ... implemented

    std::size_t count = 0;
    for (std::size_t i = 0; i < triangles.size(); ++i) {
        for (std::size_t e = 0; e < 3; ++e) {
            if (flip_if_better(i, e)) {
                ++count;
                if (stop_after_first) {
                    return count;
                }
                break;    // flipped => done with triangle
            }
        }
    }
    return count;
}

// return index of a triangle that contains p (on perimiter or interior) using
// an algorithm of your choice
//
//  precond: p lies inside bounding rectangle or on its perimeter
std::size_t Triangulation::locate(const Point &p) const {
    // ... implemented

    assert(p.x >= 0 && p.x <= width && p.y >= 0 && p.y <= height);

    // THIS IS A SLOW LINEAR SEARCH
    //  should be replaced by jump-and-walk in production code ...
    for (const Triangle &t : triangles) {
        // t.check();
        if (t.inside_test(p) <= 0) {
            return static_cast<std::size_t>(&t - &triangles[0]);
        }
    }
    std::cerr << "should not get here" << std::endl;
    assert(0 == 1);
    return 0;
}
