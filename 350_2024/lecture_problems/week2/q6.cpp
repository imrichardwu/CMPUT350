// Question
/**
 * Is there anything wrong with the following code?
 *
 * Explain what happens
 */
#include <iostream>
int &foo() {
    int x = 5;
    return x;
}

void bar() {
    int a, b, c;
    a = 1;
    b = 2;
    c = 3;
}

void test1() {
    int &y = foo();
    bar();
    std::cout << y << std::endl;
}

// Solution
int main() {
    int &y = foo();
    std::cout << &y << std::endl;    // print address of y : 0?
    std::cout << y << std::endl;     // 5? segfault!?

    bar();
    std::cout << y << std::endl;    // 1, now accessing a from bar call?
    y = 5;                          // may destroy something somewhere
    return 0;
}

/*
  g++: produces segfault (why? use g++ -g p6.cpp and gdb a.out (run/bt) to
  investigate clang++: program prints 1 ?!
*/
