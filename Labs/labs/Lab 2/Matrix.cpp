#include "Matrix.h"
#include <cassert>
#include <iostream>

// Constructor
Matrix::Matrix(int _r, int _c) {
    assert(_r > 0);
    assert(_c > 0);
    r = _r;
    c = _c;
    p = new int[r * c];
    for (int i = 0; i < r * c; i++) {
        p[i] = 0;
    }
}

// Destructor
Matrix::~Matrix() {
    delete[] p;
}

// Copy Constructor
Matrix::Matrix(const Matrix &rhs) {
    assert(rhs.r > 0);
    assert(rhs.c > 0);
    r = rhs.r;
    c = rhs.c;
    p = new int[r * c];
    for (int i = 0; i < r * c; i++) {
        p[i] = rhs.p[i];
    }
}

// Assignment Operator
Matrix &Matrix::operator=(const Matrix &rhs) {
    assert(rhs.r > 0);
    assert(rhs.c > 0);
    if (this == &rhs) {
        return *this;
    }
    delete[] p;
    r = rhs.r;
    c = rhs.c;
    p = new int[r * c];
    for (int i = 0; i < r * c; i++) {
        p[i] = rhs.p[i];
    }
    return *this;
}

// Return size 
int Matrix::size() const {
    return r * c;
}

// Set matrix 
void Matrix::set(int r, int c, int value) {
    assert(r >= 0 && r < this->r);
    assert(c >= 0 && c < this->c);
    p[r * this->c + c] = value;
}

// Return matrix 
int Matrix::get(int r, int c) const {
    assert(r >= 0 && r < this->r);
    assert(c >= 0 && c < this->c);
    return p[r * this->c + c];
}

// Print matrix
void Matrix::print() const {
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            std::cout << p[i * c + j] << " ";
        }
        std::cout << std::endl;
    }
}

// Return true iff m has the same size (rows and cols) as matrix and identical elements
bool Matrix::equals(const Matrix &m) const {
    if (r != m.r || c != m.c) {
        return false;
    }
    for (int i = 0; i < r * c; i++) {
        if (p[i] != m.p[i]) {
            return false;
        }
    }
    return true;
}
