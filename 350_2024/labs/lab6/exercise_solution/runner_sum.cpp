#include <cassert>
#include <iostream>

#define main YY_main
#define test YY_test
#include "mainSum.cpp"
#undef main
#undef test

template <int N>
void test(bool &ok, int expected) {
    std::cout << "Expecting " << expected;
    int result = Sum<N>::value;
    std::cout << " received " << result << std::endl;
    bool ok_loc = (result == expected);
    ok &= ok_loc;
}

int main() {
    bool ok = true;
    test<0>(ok, 0);
    test<10>(ok, 55);
    test<20>(ok, 210);
    test<100>(ok, 5050);

    if (ok) {
        std::cout << "PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "FAILED" << std::endl;
        return 1;
    }
}
