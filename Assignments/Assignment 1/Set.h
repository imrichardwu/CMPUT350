#ifndef Set_H
#define Set_H

#include <cstdlib>
#include <iostream>

// represents a subset of integers 0..n-1

class Set {
public:
    Set(size_t n);                   // create empty subset of {0..n-1}
    ~Set();                          // destructor
    Set(const Set &s);               // copy constructor
    Set &operator=(const Set &s);    // assignment operator

    void clear();                // empty set
    void complement();           // negate set (x was in set before <=> x is not in set after)
    bool has(size_t x) const;    // return true iff x is element of set (0 <= x < n)
    void add(size_t x);          // add element x to set (0 <= x < n)
    void remove(size_t x);       // remove element x from set (0 <= x < n)

    // add all elements of s to set; pre-cond: s.n == n
    void add(const Set &s);

    // remove all elements of s from set; pre-cond: s.n == n
    void remove(const Set &s);

    // return maximum number of elements
    size_t get_n() const {
        return n;
    }

    // print elements to output stream os (default std::cout) like so:
    // [ 1 2 4 5 ]
    // for set {1,2,4,5} (no extra spaces, no newline characters)
    void print(std::ostream &os = std::cout) const;

private:
    unsigned int *bits;    // array of integers storing bits (x in set <=> bit x = 1)
    size_t n;              // max. number of elements (elements are 0,1,...n-1)
    size_t a;              // number of allocated ints

    // you may add *private* helper functions here, or *constant* data, but no
    // other member functions or data or function implementations
};

#endif
