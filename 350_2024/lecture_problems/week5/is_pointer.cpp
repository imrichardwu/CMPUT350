#include <iostream>
using namespace std;

// default
template <typename T>
struct my_is_pointer {
    enum {
        value = 0
    };
};

// specialization for pointer types
template <typename T>
struct my_is_pointer<T *> {
    enum {
        value = 1
    };
};

int main() {
    cout << my_is_pointer<int *>::value << endl;    // 1
    cout << my_is_pointer<int>::value << endl;      // 0
    return 0;
}
