#include <iostream>
using namespace std;

struct Point {
    // 2 ways of creating Points:

    // Constructor
    Point(int x_ = 0, int y_ = 0)
        : x(x_), y(y_) {
        ++count;
    }

    // CC
    Point(const Point &rhs)
        : x(rhs.x), y(rhs.y) {
        ++count;
    }

    int x, y;

    // public static getter
    static int get_count() {
        return count;
    }

private:
    static int count;
    /* avoid global data that can be changed by anyone;
       private static class data can't
    */
};

// static variables have to be initialized in .c files
int Point::count = 0;

int main() {
    Point p;
    cout << Point::get_count() << endl;    // 1

    Point q;
    cout << Point::get_count() << endl;    // 2

    Point r(p);
    cout << Point::get_count() << endl;    // 3

    Point *x = new Point;
    cout << Point::get_count() << endl;    // 4

    delete x;

    return 0;
}
