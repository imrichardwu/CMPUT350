// mystery 2 solved:  is_bar = is_polymorphic !

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
        value = sizeof(class_tester<T>(0)) == sizeof(yes_type)
    };
};

//============================================================================

// template specialization in action

// default case: T is non-polymorphic
template <typename T, bool is_class>
struct poly_tester {
    enum {
        value = false
    };
};

// special case: T class
template <typename T>
struct poly_tester<T, true> {
    struct X : public T {    // NOLINT
        virtual ~X() = default;
    };
    struct Y : public T {};    // NOLINT

    // If T is polymorphic, sizeof(X) == sizeof(Y) because
    // the vftp is already present.
    // Otherwise, it gets added in X and the sizes are different
    // Thus, value is true if and only if T is polymorphic
    enum {
        value = sizeof(X) == sizeof(Y)
    };
};

template <typename T>
struct my_is_polymorphic    // is_polymorphic now in stdlib
{
    // distinguish two cases: T is class or not
    enum {
        value = poly_tester < T,
        my_is_class<T>::value > ::value
    };
};

struct X {};

// NOLINTNEXTLINE
struct Y {
    virtual void foo() = 0;
};

int main() {
    cout << my_is_polymorphic<int>::value << endl;    // 0
    cout << my_is_polymorphic<X>::value << endl;      // 0
    cout << my_is_polymorphic<Y>::value << endl;      // 1

    return 0;
}
