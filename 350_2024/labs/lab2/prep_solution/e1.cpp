#include <cassert>

struct Foo {
    // details immaterial
};

struct Bar {
    Foo x;
    Foo *p;    // pointer to 10 solely owned Foos

    static const int N = 10;

    // Constructor
    Bar()
        : p(new Foo[N]) {
        // x implicitly constructed here
    }

    // Destructor
    ~Bar() {
        delete[] p;
        // x implicitly destroyed here
    }

    // CC
    Bar(const Bar &rhs)
        : x(rhs.x) {
        p = new Foo[N];
        for (int i = 0; i < N; ++i) {
            p[i] = rhs.p[i];
        }
    }

    // note: first constructing and then overwriting elements by assignment can
    // be wasteful. Copy constructing arrays by using "placement new" is more
    // efficient. This technique is used by STL, but beyond the scope of this
    // course. If interested, have a look at the implementation of std::vector
    // in /usr/include/c++/...

    // AO
    Bar &operator=(const Bar &rhs) {
        if (this != &rhs) {
            x = rhs.x;
            // assume: array size always N
            // => copying OK (no need to release memory and re-allocate)
            for (int i = 0; i < N; ++i) {
                p[i] = rhs.p[i];
            }
        }
        return *this;
    }
};

int main() {
    Bar x;       // Constr
    Bar y(x);    // CC

    y = x;    // AO

    // destructors called here

    return 0;
}
