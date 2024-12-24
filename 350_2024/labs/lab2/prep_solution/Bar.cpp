#include "Bar.h"

#include "Foo.h"

#include <cassert>

// prefix Bar:: tells the compiler that we are defining
// methods for class Bar

// Constructor
Bar::Bar()
    : p(new Foo[N]) {
    // x implicitly constructed here
}

// Destructor
Bar::~Bar() {
    delete[] p;
    // x implicitly destroyed here
}

// CC
Bar::Bar(const Bar &rhs)
    : x(rhs.x)    // copy construct x from rhs.x
{
    p = new Foo[N];
    for (int i = 0; i < N; ++i) {
        p[i] = rhs.p[i];
    }
}

// AO
Bar &Bar::operator=(const Bar &rhs) {
    if (this != &rhs) {
        x = rhs.x;
        // assume: array size always N
        // => copying OK (no need to release memory first)
        for (int i = 0; i < N; ++i) {
            p[i] = rhs.p[i];
        }
    }
    return *this;
}
