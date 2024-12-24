// #define _GLIBCXX_DEBUG

#include <iostream>
#include <vector>

namespace {
void test1() {
    std::vector<int> v;
    v.push_back(123);
    auto itr = v.begin();
    std::cout << "Element at beginning of vector: " << *itr << std::endl;

    for (int i = 0; i < 20; ++i) {
        v.push_back(i);
    }
    std::cout << "Element at beginning of vector: " << *itr << std::endl;
}

void test2() {
    std::vector<int> v;
    std::cout << v[0] << std::endl;
}

template <typename T>
struct IsPointer {
    enum {
        value = 0
    };
};

template <typename T>
struct IsPointer<T *> {
    enum {
        value = 1
    };
};

}    // namespace

int main() {
    // test1();
    // test2();
    std::cout << "int is pointer: " << IsPointer<int>::value << std::endl;
    std::cout << "int* is pointer: " << IsPointer<int *>::value << std::endl;
}
