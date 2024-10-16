#include <iostream>

struct Point {
    int x{};
    int y{};
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "Point(" << p.x << ", " << p.y << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
}

int main() {
    Point p;
    std::cin >> p;
    std::cout << p << '\n';
    return 0;
}