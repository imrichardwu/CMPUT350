/**
 * Questions:
 *  1) How are objects X, Y, and Z laid out in memory
 *  2) What is the output of the following
 */

#include <iostream>
using namespace std;

#if 1
// no padding, minimizes object size
// compiler dependent
#define PACKED __attribute__((__packed__))
#else
// no effect
#define PACKED
#endif

struct X {
    int x;
};

struct PACKED Y {
    int y;
};

struct PACKED Z : public X {
    Z() {             // NOLINT
        p = new Y;    // NOLINT
    }
    virtual ~Z() {
        delete p;
    }
    virtual void foo() {}

    Y *p;    // nullptr, or sole owner of one Y object on heap
    Y y{};
};

void test1() {
    Z a;
    X *px = &a;
    cout << ((char *)px - (char *)&a) << endl;
}

void test2() {
    Z a;
    // checking sizes and class layout
    cout << "sizeof(X)=" << sizeof(X) << " sizeof(Z)=" << sizeof(Z) << endl;
    cout << "x offset=" << ((char *)&a.x - (char *)&a) << endl;
    cout << "p offset=" << ((char *)&a.p - (char *)&a) << endl;
    cout << "y offset=" << ((char *)&a.y - (char *)&a) << endl;

    X *px = &a;
    cout << ((char *)px - (char *)&a) << endl;
}

int main() {
    test1();
    // test2();
}

// Solution:
/*
  offsets indicate the following layout:

    X                    Y
  +----+               +----+
  | x  | 4 bytes       | y  | 4 bytes
  +----+               +----+

       Z    (packed)
  a +-----+
    |VFTP | 8 bytes
    +-----+<------------- X *px = &a;  pointing to non-polymorphic
    |  x  | 4 bytes                    sub-object x
    +-----+
    |  p  | 8 bytes       So, px and &a are actually different
    +-----+               memory addresses (off by 8 bytes).
    |  y  | 4 bytes       Internally, when converting a Z pointer
    +-----+               into an X pointer, 8 gets added to skip
                          Z's VFTP. This shouldn't be surprising
                          because px has to point to a valid X object,
                          which doesn't have a VFTP
*/
