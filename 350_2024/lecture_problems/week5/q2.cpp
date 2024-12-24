#include <iostream>

using namespace std;

// === previous problem ===

using yes_type = int;
using no_type = char;    // different sizes!

template <typename T>
yes_type class_tester(void (T::*)());

template <typename T>
no_type class_tester(...);

template <typename T>
struct my_is_class    // is_class now in stdlib
{
    enum {
        value = sizeof(class_tester<T>(nullptr)) == sizeof(yes_type)
    };
};

//============================================================================

// template specialization in action

template <typename T, bool is_class>
struct bar_tester {
    enum {
        value = false
    };
};

template <typename T>
struct bar_tester<T, true> {
    struct X : public T {    // NOLINT
        virtual ~X() = default;
    };
    struct Y : public T {};    // NOLINT

    enum {
        value = sizeof(X) == sizeof(Y)
    };
};

template <typename T>
struct is_bar {
    enum {
        value = bar_tester < T,
        my_is_class<T>::value > ::value
    };
};

struct X {};

// NOLINTNEXTLINE
struct Y {
    virtual void foo() = 0;
};

int main() {
    cout << is_bar<int>::value << endl;    // 0
    cout << is_bar<X>::value << endl;      // 0
    cout << is_bar<Y>::value << endl;      // 1

    return 0;
}
