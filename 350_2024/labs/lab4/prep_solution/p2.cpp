#include <iostream>
using namespace std;

struct Point {
    int x{};
    int y{};

    Point(int x, int y)
        : x(x), y(y) {}

    bool operator==(const Point &rhs) {
        return x == rhs.x && y == rhs.y;
    }

    bool operator>(const Point &rhs) {
        if (x > rhs.x) {
            return true;
        }

        if (x < rhs.x) {
            return false;
        }

        return y > rhs.y;
    }

    bool operator>=(const Point &rhs) {
        if (x > rhs.x) {
            return true;
        }

        if (x < rhs.x) {
            return false;
        }

        return y >= rhs.y;
    }
};

int main() {
    Point p(0, 1), q(2, 3);

    cout << (p == q) << " " << (p > q) << " " << (p >= q) << endl;

    return 0;
}
