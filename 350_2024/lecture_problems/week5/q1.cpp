#include <iostream>

using yes_type = int;
using no_type = char;

template <typename T>                  // takes pointer to member function of class T that
yes_type foo_tester(void (T::*)());    // takes no argument and returns nothing

template <typename T>       // match any argument type (this is a variadic
no_type foo_tester(...);    // function that accepts arbitrary parameter lists)

template <typename T>
struct is_foo {
    enum {
        value = sizeof(foo_tester<T>(nullptr)) == sizeof(yes_type)
    };
};

struct X {};

// Question
// What does is_foo<T>::value represent?
int main() {
    std::cout << is_foo<int>::value;         // 0
    std::cout << is_foo<double *>::value;    // 0
    std::cout << is_foo<X>::value;           // 1
    return 0;
}
