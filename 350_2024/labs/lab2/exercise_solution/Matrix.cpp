//  
#include "Matrix.h"

#include <cassert>
#include <cstdio>
#include <iostream>

using namespace std;

//  construct n by n matrix and initialize elements with 0
//  pre-condition: n > 0
Matrix::Matrix(int _r, int _c)
    : r(_r), c(_c) {
    assert(r > 0 && c > 0);
    p = new int[r * c]{};    // {} initializes POD with 0
                             // alternatively: loop 0..n*n-1 and set each element to 0
                             // puts(""); // for finding line in g++ -S -O3 ... output
}

// release all resources
Matrix::~Matrix() {
    delete[] p;
}

// CC
Matrix::Matrix(const Matrix &rhs)
    : r(rhs.r), c(rhs.c) {
    int s = r * c;
    //  allocate new array and copy rhs values over
    p = new int[s];
    for (int i = 0; i < s; ++i) {
        p[i] = rhs.p[i];
    }
}

// AO
// pre-condition: rhs must have same size
Matrix &Matrix::operator=(const Matrix &rhs) {
    if (&rhs != this) {
        assert(r == rhs.r && c == rhs.c);

        // sizes compatible =>
        // just copy all elements over
        int s = r * c;
        for (int i = 0; i < s; ++i) {
            p[i] = rhs.p[i];
        }
    }
    return *this;
}

// set matrix element [r,c] to value
// (indexes start with 0)
// pre-condition: r,c within range
void Matrix::set(int r, int c, int value) {
    assert(0 <= r && r < this->r);
    assert(0 <= c && c < this->c);
    p[r * this->c + c] = value;
}

// return size (number of total elements)
int Matrix::size() const {
    return r * c;
}

// return matrix element [r,c] (indexes start with 0)
// pre-condition: r,c within range
int Matrix::get(int r, int c) const {
    assert(0 <= r && r < this->r);
    assert(0 <= c && c < this->c);
    return p[r * this->c + c];
}

// print all elements to stdout - row by row, using space as separator
void Matrix::print() const {
    for (int r = 0; r < this->r; ++r) {
        for (int c = 0; c < this->c; ++c) {
            cout << get(r, c) << " ";
        }
        cout << endl;
    }
}

// return true iff m has the same dimension as maxtrix and identical elements
bool Matrix::equals(const Matrix &m) const {
    if (r != m.r || c != m.c) {
        return false;
    }

    int s = r * c;

    for (int i = 0; i < s; ++i) {
        if (p[i] != m.p[i]) {
            return false;
        }
    }
    return true;
}
