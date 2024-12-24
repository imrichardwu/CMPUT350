#include "Triang.h"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>

using namespace std;

/*
  for points a,b,c
    return > 0 if a,b,c are ordered in counter-clockwise order
    return = 0 if a,b,c are colinear
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

    abdet = adx * bdy - bdx * ady;
    bcdet = bdx * cdy - cdx * bdy;
    cadet = cdx * ady - adx * cdy;
    alift = adx * adx + ady * ady;
    blift = bdx * bdx + bdy * bdy;
    clift = cdx * cdx + cdy * cdy;

    return alift * bcdet + blift * cadet + clift * abdet;
}

// add point and repair triangulation
// precond: p must lie in the interior of the
// bounding rectangle
void Triangulation::add_point(const Point &p) {
    assert(p.x > 0 && p.x < width && p.y > 0 && p.y < height);
    std::size_t l = locate(p);
    Coordinate ins = triangles[l].inside_test(p);

    if (ins < 0) {
        /* interior: split triangle into three triangles (A,B,C)

            1
            /\
        n0 / B\ n1    add 2 (B,C) and recycle original triangle (now A)
          /A p \
         0--C---2
            n2

          important: ensure clockwise order

        */

        triangles.push_back(Triangle());
        triangles.push_back(Triangle());

        // triangle indexes
        std::size_t indA = l;
        std::size_t indB = triangles.size() - 2;
        std::size_t indC = indB + 1;

        // triangles
        Triangle &tA = triangles[indA];    // recycled
        Triangle &tB = triangles[indB];
        Triangle &tC = triangles[indC];

        // assign points
        tB.points[0] = tA.points[1];
        tB.points[1] = tA.points[2];
        tB.points[2] = p;

        tC.points[0] = tA.points[2];
        tC.points[1] = tA.points[0];
        tC.points[2] = p;

        tA.points[2] = p;

        // assign neighbors of new triangles

        tB.neighbors[0] = tA.neighbors[1];
        tB.neighbors[1] = indC;
        tB.neighbors[2] = indA;

        tC.neighbors[0] = tA.neighbors[2];
        tC.neighbors[1] = indA;
        tC.neighbors[2] = indB;

        // tA.neighbors[0] unchanged
        tA.neighbors[1] = indB;
        tA.neighbors[2] = indC;

        // update neighbors of previous neighbors of triangle l
        // neighbor, orig index, new index
        set_neighbor(tB.neighbors[0], l, indB);
        set_neighbor(tC.neighbors[0], l, indC);

        tA.check();
        tB.check();
        tC.check();

    } else {
        // perimiter

        std::cout << "P";

        Triangle &t = triangles[l];

        for (std::size_t i = 0; i < 3; ++i) {
            if (p == t.points[i]) {
                return;    // p exists already
            }
        }

        // split two adjacent triangles into two

        // on which edge does p lie ?

        std::size_t e{};
        for (e = 0; e < 3; ++e) {
            if (orientation_test(t.points[e], t.points[(e + 1) % 3], p) == 0) {
                break;
            }
        }

        // t.print();
        // p.print();
        // std::cout << std::endl;

        assert(e < 3);
        assert(t.neighbors[e] != Triangle::BORDER);

        std::size_t other = t.neighbors[e];

        triangles.push_back(Triangle());
        triangles.push_back(Triangle());

        std::size_t new_l = triangles.size() - 2;
        std::size_t new_other = triangles.size() - 1;

        split_triangle(l, new_l, p, other, new_other);
        split_triangle(other, new_other, p, l, new_l);

        triangles[l].check();
        triangles[other].check();
    }
}

void Triangulation::set_neighbor(std::size_t neighbor_ind, std::size_t old_ind, std::size_t new_ind) {
    if (neighbor_ind != Triangle::BORDER) {
        std::size_t i{};
        for (i = 0; i < 3; ++i) {
            if (triangles[neighbor_ind].neighbors[i] == old_ind) {
                break;
            }
        }
        assert(i < 3);

        triangles[neighbor_ind].neighbors[i] = new_ind;
    }
}

void Triangulation::split_triangle(
    std::size_t ind,
    std::size_t new_ind,
    const Point &p,
    std::size_t other,
    std::size_t new_other
) {
    /*
                   /|\
      new_other   / i \i+1  ind     (A)
                 /  |__\
                 \ p|  /
      other       \ i /i+2  new_ind (B)
                   \|/
    */

    // on which side does p lie ?

    Triangle &A = triangles[ind];
    Triangle &B = triangles[new_ind];
    std::size_t i{};
    for (i = 0; i < 3; ++i) {
        if (orientation_test(A.points[i], A.points[(i + 1) % 3], p) == 0) {
            break;
        }
    }
    assert(i < 3);
    assert(A.neighbors[i] != Triangle::BORDER);

    B = A;

    // rewire A
    A.points[i] = p;
    A.neighbors[(i + 2) % 3] = new_ind;
    A.neighbors[i] = new_other;

    // rewire B
    B.points[(i + 1) % 3] = p;
    B.neighbors[(i + 1) % 3] = ind;
    B.neighbors[i] = other;
    set_neighbor(B.neighbors[(i + 2) % 3], ind, new_ind);
}

// consider triangle with index t_ind and edge with index e
// flip edge iff this increases the minimum interior angle locally
// return true if edge was flipped, false otherwise
bool Triangulation::flip_if_better(size_t t_ind, std::size_t e) {
    Triangle &t = triangles[t_ind];
    if (t.neighbors[e] == Triangle::BORDER) {
        return false;
    }

    // find fourth point

    size_t s_ind = t.neighbors[e];
    Triangle &s = triangles[s_ind];

    s.check();
    t.check();

    std::size_t i{};
    for (i = 0; i < 3; ++i) {
        if (s.points[i] != t.points[e] && s.points[i] != t.points[(e + 1) % 3]) {
            break;
        }
    }

    // circle test: fourth point outside => OK
    if (circle_test(t.points[0], t.points[1], t.points[2], s.points[i]) >= 0) {
        return false;
    }

    // otherwise flip

    /*    e+1:i+1                    :i+1
       A  /|\  B                  A  / \  B
         / | \                      / s \
    :i  /  |  \e+2  -flip->  e+1:i /_____\ e+2:i+2
        \ s|t /                    \     /
         \ | /                      \ t /
       C  \|/  D                  C  \ /  D
           e:i+2                      e

          point in t : point in s (mod 3)
     */

#if 0  
  cout << t_ind << " " << s_ind << endl;
  cout << "before D:" << endl;
  t.print();
  cout << endl;
  s.print();
  cout << endl;
#endif

    // rewire triangles

    // triangles A,D unchanged

    size_t B = t.neighbors[(e + 1) % 3];
    size_t C = s.neighbors[(i + 2) % 3];

    // changes in t

    t.points[(e + 1) % 3] = s.points[i];
    t.neighbors[e] = C;
    set_neighbor(C, s_ind, t_ind);
    t.neighbors[(e + 1) % 3] = s_ind;

    // changes in s

    s.points[(i + 2) % 3] = t.points[(e + 2) % 3];
    s.neighbors[(i + 1) % 3] = B;
    set_neighbor(B, t_ind, s_ind);
    s.neighbors[(i + 2) % 3] = t_ind;

#if 0  
  cout << "after D:" << endl;
  t.print();
  cout << endl;
  s.print();
  cout << endl;
#endif

    s.check();
    t.check();
    return true;
}
