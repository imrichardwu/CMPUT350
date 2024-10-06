#include "Bar.h"

int main() {
    Bar x;
    Bar *p = new Bar[10];
    delete[] p;
    x = x;

    Bar y(x);
    y = x;

    // destructors called here

    return 0;
}
