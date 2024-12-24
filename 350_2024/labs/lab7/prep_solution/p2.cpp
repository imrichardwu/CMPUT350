#include <cstdlib>
#include <iostream>
#include <set>

using namespace std;

int main() {
    // set of integers represented as binary search tree
    // => iterating through s visits elements in non-decreasing order
    set<int> s;

    for (;;) {
        int x{};
        cin >> x;
        if (!cin) {
            break;
        }
        s.insert(x);
    }

    if (!cin.eof()) {
        cerr << "input error" << endl;
        std::exit(1);
    }

    // print encountered ints in non-decreasing order
    for (int x : s) {
        cout << x << " ";
    }

    cout << endl;
    return 0;
}

/*
  runtime:

  inserting into a set runs in O(log m) time when m elements are stored.

  This means that the first loop runs in time O(n log n), because each
  insertion runs in time O(log n).

  Traversing through s takes linear time in n.

  Total time: O(n log n)

  A more strict analysis results in worst case runtime Θ(n log n).
*/
