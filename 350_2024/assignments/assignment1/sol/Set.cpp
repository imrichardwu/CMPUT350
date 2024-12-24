#include "Set.h"

#include <algorithm>    // for std::copy and std::fill
#include <cassert>

Set::Set(size_t n_) : n(n_) {
    assert(n > 0);
    a = (n + INT_BITS - 1) / INT_BITS;    // = ceil(n / INT_BITS)
    //  e.g.: INT_BITS=32
    //  n = 32 * k                  => a = k
    //  n = 32 * k + r (0 < r < 32) => a = k+1
    bits = new unsigned int[a];
    clear();
}

Set::~Set() {
    delete[] bits;
}

Set::Set(const Set &rhs) : bits(new unsigned int[rhs.a]), n(rhs.n), a(rhs.a) {
    // copy range [begin, end) -> to
    // POD: uses memcpy, non-POD: element by element (assignment op)
    std::copy(rhs.bits, rhs.bits + a, bits);
}

Set &Set::operator=(const Set &rhs) {
    if (this == &rhs) {
        return *this;    // self assignment
    }

    if (a != rhs.a) {                      // if array sizes different
        delete[] bits;                     // release previously allocated memory
        bits = new unsigned int[rhs.a];    // and allocate new array
    }

    n = rhs.n;
    a = rhs.a;
    std::copy(rhs.bits, rhs.bits + a, bits);
    return *this;
}

void Set::clear() {
    // fill [from, to) with value  (a elements in this case)
    std::fill(bits, bits + a, 0);
}

void Set::complement() {
    for (size_t i = 0; i < a; ++i) {
        bits[i] = ~bits[i];
    }
}

unsigned int Set::bit_mask(size_t x) const {
    // Make sure x is within [0,n)
    assert(x < n);
    // this is equivalent to 1 << (x % INT_BITS)
    // but faster if INT_BITS is a power of 2 (which it is)
    return 1U << (x & (INT_BITS - 1));
}

// return true iff x is element of set (0 <= x < n)
bool Set::has(size_t x) const {
    // / rounds towards 0
    // good compilers recognize powers of 2 and replace /2(^k) by >> k
    // So, no need for us to do that and make the code harder to read
    return (bits[x / INT_BITS] & bit_mask(x)) != 0;
}

// add element x to set (0 <= x < n)
void Set::add(size_t x) {
    bits[x / INT_BITS] |= bit_mask(x);
}

// remove element x from set (0 <= x < n)
void Set::remove(size_t x) {
    bits[x / INT_BITS] &= ~bit_mask(x);
}

// add all elements of s to set; pre-cond: s.n == n
void Set::add(const Set &s) {
    assert(n == s.n);

    for (size_t i = 0; i < a; ++i) {
        bits[i] |= s.bits[i];
    }
}

// remove all elements of s from set; pre-cond: s.n == n
void Set::remove(const Set &s) {
    assert(n == s.n);

    for (size_t i = 0; i < a; ++i) {
        bits[i] &= ~s.bits[i];
    }
}

// print elements to output stream os (default std::cout) like so:
// [ 1 2 4 5 ]
// for set {1,2,4,5} (no extra spaces, no newline characters)
void Set::print(std::ostream &os) const {
    os << "[ ";
    for (size_t i = 0; i < n; ++i) {
        if (has(i)) {
            os << i << " ";
        }
    }
    os << "]";
}
