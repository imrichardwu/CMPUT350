#include <iostream>

// a point with integer coordinates

struct Point {
    // constructor
    Point() {
        x = y = 0;
    }

    // add point componentwise
    void add(const Point &p) {
        x += p.x;
        y += p.y;
    }

    // print to standard output
    void print() const {
        std::cout << "[" << x << "," << y << "]" << std::endl;
    }

    // data
    int x, y;
};

int main() {
    const int N = 200;
    Point *A = new Point[N], sum;

    for (int i = 0; i < N; ++i) {
        A[i].x = i;
        A[i].y = -i;
        sum.add(A[i]);
    }
    sum.print();
    delete[] A;

    return 0;
}
