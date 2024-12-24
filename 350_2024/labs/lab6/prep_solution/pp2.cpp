// requires C++11's static_assert(cond, msg) which checks Boolean condition
// cond at COMPILE TIME, and prints msg if false

#include <iostream>

using namespace std;

template <typename T>
struct is_fp {
    enum {
        value = 0
    };    // default
};

template <>
struct is_fp<float> {
    enum {
        value = 1
    };
};

template <>
struct is_fp<double> {
    enum {
        value = 1
    };
};

template <>
struct is_fp<long double> {
    enum {
        value = 1
    };
};

template <typename T>
struct Foo {
    // fails at COMPILE TIME if T is a floating point type
    static_assert(!is_fp<T>::value, "T must not be a floating point type");

    Foo() = default;    // This is the same as writting Foo() {}
};

int main() {
    cout << is_fp<long double>::value << endl;    // 1
    cout << is_fp<double>::value << endl;         // 1
    cout << is_fp<float>::value << endl;          // 1
    cout << is_fp<int>::value << endl;            // 0

    // Foo<double> f; // error
    [[maybe_unused]] Foo<int> g;    // ok
    return 0;
}
