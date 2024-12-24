#include <cassert>
#include <cstddef>
#include <iostream>

using namespace std;

template <typename T, size_t N>
class Array {
    T a[N];

public:
    Array() {
        // note: code below initializes non-POD types a second time
        // (using an assignment), which can be SLOW
        // this can be avoided by using T a[N]{}; above  (see Array2)
        for (size_t i = 0; i < N; ++i) {
            a[i] = T{};    // 0 for POD
        }
    }

    size_t size() const {
        return N;
    }

    T &operator[](size_t i) {
        assert(i < N);
        return a[i];
    }

    const T &operator[](size_t i) const {
        assert(i < N);
        return a[i];
    }
};

template <typename T, size_t N>
class Array2 {
    T a[N]{};    // avoid repeated construction for POD and non-POD types
                 // calls empty constructor for non-POD, and assigns 0 in POD case
                 // see https://en.cppreference.com/w/cpp/language/list_initialization
                 //
                 // note: T a[N](); doesn't work here, because this is seen as an
                 // array of functions
                 // Hence we use uniform initialization
public:
    size_t size() const {
        return N;
    }

    T &operator[](size_t i) {
        assert(i < N);
        return a[i];
    }

    const T &operator[](size_t i) const {
        assert(i < N);
        return a[i];
    }
};

int main() {
    Array<int, 3> A1;
    Array<double, 5> A2;
    Array2<short, 5> A3;

    A1[0] = 2;

    cout << A1.size() << endl;
    cout << A2.size() << endl;
    cout << A3.size() << endl;

    cout << A1[0] << endl;

    return 0;
}
