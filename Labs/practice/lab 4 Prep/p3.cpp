#include <iostream>

struct Point {
    int x{};
    int y{};
};

Point &operator++(Point &p, int) {
    p.x++;
    p.y++;
    return p;
}

Point &operator++(Point &p) {
    ++p.x;
    ++p.y;
    return p;
}

Point &operator--(Point &p, int) {
    p.x--;
    p.y--;
    return p;
}

Point &operator--(Point &p) {
    --p.x;
    --p.y;
    return p;
}



int main() {
    Point p; 
    std::cin >> p.x >> p.y;
    p--;
    std::cout << p.x << std::endl;
    return 0;
}