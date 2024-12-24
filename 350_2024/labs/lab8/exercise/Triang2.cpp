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
    // ... implement ...
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
Triangulation::Triangulation(Coordinate w, Coordinate h) {
    // ... implement ...
}

// Check every triangle once by visiting all of its edges and flipping those
// that increase the minimum interior angle locally.
// If flag is set, stop after first flip.
// return number of flipped edges (0 => done)
//
// Hint: use flip_if_better()
//
std::size_t Triangulation::Delaunay_pass(bool stop_after_first) {
    // ... implement ...
}

// return index of a triangle that contains p (on perimiter or interior) using
// an algorithm of your choice
//
// pre-cond: p lies inside bounding rectangle or on its perimeter
//
std::size_t Triangulation::locate(const Point &p) const {
    // ... implement ...
}
