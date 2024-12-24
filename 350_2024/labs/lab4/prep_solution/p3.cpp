#include <iostream>
using namespace std;

struct Point {
    int x{};
    int y{};

    // ++/-- : add/subtract 1 to/from x

    // pre ++/-- : return reference to variable for chaining

    Point &operator++() {
        ++x;
        return *this;
    }

    Point &operator--() {
        --x;
        return *this;
    }

    // post ++/-- : return copy of previous state

    Point operator++(int) {
        Point ret(*this);
        ++x;
        return ret;
    }

    Point operator--(int) {
        Point ret(*this);
        --x;
        return ret;
    }

    void print() {
        cout << x << " " << y << endl;
    }
};

int main() {
    Point p = {0, 0};

    cout << "test pre++" << endl;

    Point a = ++p;
    a.print();
    p.print();
    cout << endl;

    cout << "test pre--" << endl;

    Point b = --p;
    b.print();
    p.print();
    cout << endl;

    cout << "test post++" << endl;

    Point c = p++;
    c.print();
    p.print();
    cout << endl;

    cout << "test post--" << endl;

    Point d = p--;
    d.print();
    p.print();
    cout << endl;

    return 0;
}
