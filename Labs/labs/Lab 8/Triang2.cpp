#include "Triang.h"

#include <cassert>

// return < 0 if point p lies in the interior
// return = 0 if point p lies on the perimeter
// return > 0, otherwise
// assumes triangle points in clockwise order
//
// Hint: use orientation() exactly 3 times and distinguish cases given by the
//       number of results <,=,> 0
//
int Triangle::inside_test(const Point &p) const {
    size_t num_negative = 0, num_zero = 0, num_positive = 0;

    for (size_t i = 0; i < 3; ++i) {
        Coordinate o = orientation_test(points[i], points[(i + 1) % 3], p); // Orientation of triangle points
        if (o < 0) { // For clockwise orientation
            ++num_negative;
        } else if (o == 0) { // For colinear points
            ++num_zero;
        } else { // For counter-clockwise orientation
            ++num_positive;
        }
    }

    if (num_positive > 0) {
        return 1; // Outside
    } else if (num_zero > 0) {
        return 0; // On perimeter
    } else {
        return -1; // Inside
    }
}


// store dimensions and triangulate bounding rectangle
// (0,0)->(w,0)->(w,h)->(0,h)->(0,0)
// use BORDER as index of out-of-bounds neighbors (see Triangle Mesh
// Representation on lab page)
// precond: w, h > 0
//
// Hint: at the end the triangulation must contain two triangles which
// together represent the rectangle. Both triangles must be oriented
// clockwise - which can be checked with Triangle::check()
//
//          BORDER
// (0,h)    _|_ (w,h)
//         |  /|
// BORDER<-| x |->BORDER
//         |/__|
// (0,0)     |  (w,0)
//          BORDER
//
Triangulation::Triangulation(Coordinate w, Coordinate h) : width(w), height(h) {
    assert(w > 0 && h > 0);

    // Create corner points
    Point p0(0, 0);
    Point p1(w, 0);
    Point p2(w, h);
    Point p3(0, h);

    // Triangle t0: p2, p1, p0 (clockwise)
    Triangle t0;
    t0.points[0] = p2;
    t0.points[1] = p1;
    t0.points[2] = p0;
    t0.neighbors[0] = Triangle::BORDER; // Edge from p2 to p1
    t0.neighbors[1] = Triangle::BORDER; // Edge from p1 to p0
    t0.neighbors[2] = 1; // Neighboring triangle (t1)
    t0.check();

    // Triangle t1: p3, p2, p0 (clockwise)
    Triangle t1;
    t1.points[0] = p3;
    t1.points[1] = p2;
    t1.points[2] = p0;
    t1.neighbors[0] = 0; // Neighboring triangle (t0)
    t1.neighbors[1] = Triangle::BORDER; // Edge from p2 to p3
    t1.neighbors[2] = Triangle::BORDER; // Edge from p3 to p0
    t1.check();

    triangles.push_back(t0);
    triangles.push_back(t1);
}



// Check every triangle once by visiting all of its edges and flipping those
// that increase the minimum interior angle locally.
// If flag is set, stop after first flip.
// return number of flipped edges (0 => done)
//
// Hint: use flip_if_better()
//
std::size_t Triangulation::Delaunay_pass(bool stop_after_first) {
    std::size_t num_flipped = 0; // Number of flipped edges

    for (std::size_t i = 0; i < triangles.size(); ++i) { // Iterate over all triangles
        for (std::size_t e = 0; e < 3; ++e) {   // Iterate over all edges of the triangle
            if (flip_if_better(i, e)) { // Flip edge if it increases the minimum interior angle locally
                ++num_flipped;  // Increment number of flipped edges
                if (stop_after_first) { // If flag is set, stop after first flip
                    return num_flipped;
                }
            }
        }
    }
    return num_flipped;
}


// return index of a triangle that contains p (on perimiter or interior) using
// an algorithm of your choice
//
// pre-cond: p lies inside bounding rectangle or on its perimeter
//
std::size_t Triangulation::locate(const Point &p) const {
    // Simple linear search
    for (std::size_t i = 0; i < triangles.size(); ++i) {
        if (triangles[i].inside_test(p) <= 0) { // Check if point p lies inside the triangle
            return i;
        }
    }
    // Should not reach here; p must be inside bounding rectangle
    assert(false && "Point not found in any triangle");
    return 0;
}

