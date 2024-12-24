// mystery 1 solved:  is_foo = is_class

#include <iostream>
#include <type_traits>

// using namespace std;

using yes_type = int;
using no_type = char;    // different sizes!

// takes pointer to member function of class T that takes no argument and
// returns nothing (1)
template <typename T>
yes_type class_tester(void (T::*)());

// match any argument type (this is a so-called variadic function that
// accepts arbitrary lists of parameters) (2)
template <typename T>
no_type class_tester(...);

/*
  =>

  if T is a class: sizeof(class_tester<T>(nullptr)) == sizeof(yes_type); because
  then nullptr can be regarded a T member function pointer and function (1) matches
  (because it is more specialized than function (2)

  if T is not a class: sizeof(class_tester<T>(nullptr)) == sizeof(no_type); because
  T can't have a member function pointer and function (2) matches
*/

template <typename T>
struct is_class {
    enum {
        value = sizeof(class_tester<T>(nullptr)) == sizeof(yes_type)
    };
};

struct X {};
union Y {};

int main() {
    std::cout << is_class<int>::value << std::endl;         // 0
    std::cout << is_class<double *>::value << std::endl;    // 0
    std::cout << is_class<X>::value << std::endl;           // 1
    std::cout << is_class<Y>::value << std::endl;           // 1
    std::cout << std::is_class<Y>::value << std::endl;      // 0

    // so: is_class::value == 1 if and only if T is a class type
    // (including unions, as they can have member functions)
    //
    // note: C++11's std::is_class defined in <type_traits> excludes unions

    return 0;
}
