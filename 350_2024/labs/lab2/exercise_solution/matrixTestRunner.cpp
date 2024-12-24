#include "Matrix.h"

#include <iostream>

using namespace std;

int main() {
    const int ROWS = 5;
    const int COLS = 4;
    Matrix u(ROWS, COLS);

    // should be all 0s
    std::cout << "expecting 5x4 matrix of all 0s" << std::endl;
    u.print();
    cout << endl;

    Matrix v(ROWS, COLS);
    // increasing values
    int k = 0;
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            v.set(r, c, k++);
        }
    }

    std::cout << "expecting 5x4 matrix of all increasing values" << std::endl;
    v.print();

    //  test equals

    if (!u.equals(u)) {
        cout << "Error: equals oops 1" << endl;
    }
    if (u.equals(v)) {
        cout << "Error: equals oops 2" << endl;
    }
    if (!v.equals(v)) {
        cout << "Error: equals oops 3" << endl;
    }

    //  test CC
    Matrix w(u);

    if (!w.equals(u)) {
        cout << "Error: CC oops" << endl;
    }

    // test AO
    Matrix z(ROWS, COLS);

    z = u;
    if (!z.equals(u)) {
        cout << "Error: AO oops" << endl;
    }

    return 0;
}
