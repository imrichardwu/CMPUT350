#include <iostream>

struct Point {
    int x{};
    int y{};
};

bool operator==(const Point &left, Point &right) {
    if (left.x == right.x && left.y == right.y) {
        return true;
    }
    return false;
}

bool operator>(const Point &left, const Point &right) {
    if (left.x > right.x) {
        return true;
    } else if (left.x == right.x) {
        return left.y > right.y;
    } else {
        return false;
    }
}

bool operator>=(const Point &left, const Point &right) {
    if (left.x > right.y) {
        return true;
    } else if (left.y == right.y) {
        return true;
    }
    return false;
}

int main() {
    Point p1, p2;
    std::cout << "Enter first point coordinates (x y): ";
    std::cin >> p1.x >> p1.y;

    std::cout << "Enter second point coordinates (x y): ";
    std::cin >> p2.x >> p2.y;
    
    if(p1 == p2) {
        std::cout << "p1 is equal to p2" << std::endl;
    } else {
        std::cout << "p1 is not equal to p2" << std::endl;
    }
    
    if (p1 >=p2) {
        std::cout << "p1 is greater or equal to p2" << std::endl;
    } else {
        std::cout << "p2 is greater or equal to p1" << std::endl;
    }

    if (p1 > p2) {
        std::cout << "p1 is greater than p2" << std::endl;
    } else {
        std::cout << "p2 is greater than p1" << std::endl;
    }

    return 0;
}