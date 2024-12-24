// read n > 0 points and print the last n-1 in
// order closest to farthest from first point

#include <cassert>

#include "Sorter.h"

using namespace std;

int main() {
    double x0, y0;
    Sorter ps;

    // read origin
    cin >> x0 >> y0;

    assert(cin);    // pre-cond check: ensure read succeeded

    // read other points
    for (;;) {
        double x, y;

        cin >> x >> y;
        if (!cin) {
            break;    // end of input or error
        }

        ps.add_point(x, y);
    }

    // print all points
    // closest to farthest from origin
    ps.print_sorted_closest_to(x0, y0);
}
