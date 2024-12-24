#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// generic selection sort
// sorts range by using functor less
template <typename Iterator, typename Comp>
void selection_sort(Iterator begin, Iterator end, Comp less) {
    for (Iterator it = begin; it != end; ++it) {
        // find minimal element and swap with first
        // then proceed
        Iterator it_min = it, it2 = it;
        ++it2;
        for (; it2 != end; ++it2) {
            if (less(*it2, *it_min)) {
                it_min = it2;
            }
        }
        swap(*it_min, *it);
    }
}

struct MyLess {
    bool operator()(int a, int b) {
        return a < b;
    }
};

bool my_less(int a, int b) {
    return a < b;
}

struct MyGreater {
    bool operator()(int a, int b) {
        return a > b;
    }
};

int main() {
    // C++11 feature: initializer lists
    // this creates a vector with 4 elements
    vector<int> v{1, 2, 3, 4};

    selection_sort(begin(v), end(v), MyLess());
    for (auto x : v) {
        cout << x << " ";
    }
    cout << endl;
    // 1 2 3 4

    selection_sort(begin(v), end(v), MyGreater());
    for (auto x : v) {
        cout << x << " ";
    }
    cout << endl;
    // 4 3 2 1

    // a bit easier with C++11's lambda functions (see part7, 2nd half)
    selection_sort(begin(v), end(v), [](const int a, const int b) { return a > b; });

    // trigger STL runtime error (comparing iterators from different containers)
    // when using -D_GLIBCXX_DEBUG
    vector<int> u{1, 2, 3, 4};
    selection_sort(begin(u), end(v), MyGreater());

    // also works with arrays and comparison FUNCTIONS
    int A[] = {1, 2, 3, 4};
    selection_sort(A, A + 4, my_less);

    return 0;
}
