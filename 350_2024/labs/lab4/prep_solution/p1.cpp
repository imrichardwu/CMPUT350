#include <iostream>
using namespace std;

struct Point {
    int x{};
    int y{};
};

ostream &operator<<(ostream &os, const Point &p) {
    // returns result of << (a reference to an ostream)
    // could also write: return os; after output
    return os << p.x << " " << p.y;
}

istream &operator>>(istream &is, Point &p) {
    // returns result of >> (a reference to an istream)
    // could also write: return is; after input
    return is >> p.x >> p.y;
}

int main() {
    Point p;
    cin >> p;
    cout << p << endl;

    return 0;
}
