#pragma once

class Matrix {
public:
    // construct r by c matrix and initialize elements with 0
    // pre-condition: _r > 0, _c > 0
    Matrix(int _r, int _c);

    // release all resources
    ~Matrix();

    // CC
    Matrix(const Matrix &rhs);

    // AO
    // pre-condition: rhs must have same shape
    Matrix &operator=(const Matrix &rhs);

    // return size (number of total elements)
    int size() const;

    // set matrix element (r,c) to value
    // (indices start with 0)
    // pre-condition: r,c within range
    void set(int r, int c, int value);

    // return matrix element (r,c)
    // (indexes start with 0)
    // pre-condition: r,c within range
    int get(int r, int c) const;

    // print all elements to stdout - row by row, using space as separator
    void print() const;

    // return true iff m has the same size (rows and cols) as maxtrix and identical elements
    bool equals(const Matrix &m) const;

private:
    // data
    int r;     // rows
    int c;     // cols
    int *p;    // sole owner of r*c ints
};
