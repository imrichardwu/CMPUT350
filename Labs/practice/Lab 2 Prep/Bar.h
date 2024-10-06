#ifndef BAR_H_
#define BAR_H_

#include "Foo.h"

struct Bar {
    // constructor
    Bar();

    // destructor
    ~Bar();

    // CC
    Bar(const Bar &rhs);

    // AO
    Bar &operator=(const Bar &rhs);

    // data
    static const int N = 10;
    Foo x;
    Foo *p;    // always pointing to N solely owned Foos
};

#endif    // BAR_H_
