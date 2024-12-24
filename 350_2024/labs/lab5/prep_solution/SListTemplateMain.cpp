// test code for written SList class template

#include <iostream>

#include "SListTemplate.h"

using namespace std;

struct Pair {
    Pair(int x = 0, int y = 0) : x(x), y(y) {}

    int x, y;
};

ostream &operator<<(ostream &os, const Pair &p) {
    os << p.x << " " << p.y;
    return os;
}

int main() {
    // all must work:
    //  SList<double> list;
    //  SList<int> list;
    SList<Pair> list;

    for (int i = 0; i < 10; ++i) {
        list.add_head(i);
    }

    // 9 8 7 ... 0   (or 9 0 8 0 ... 0 0)
    while (list.get_head()) {
        cout << list.get_head()->data << endl;
        list.remove_head();
    }

    return 0;
}
