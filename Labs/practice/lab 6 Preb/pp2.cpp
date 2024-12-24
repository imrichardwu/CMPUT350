#include <iostream>
#include <cassert>

template <typename T> 
class is_fp {
public:
    static constexpr bool value = false;
};

template <>
class is_fp<float> {
public:
    static constexpr bool value = true;
};

template <>
class is_fp<double> {
public:
    static constexpr bool value = true;
};

int main() {
    std::cout << is_fp<int>::value << std::endl;
    std::cout << is_fp<float>::value << std::endl;
    std::cout << is_fp<double>::value << std::endl;
    std::cout << is_fp<long>::value << std::endl;
}