// Question
/**
 * Write a function "apply_func" which takes:
 *   - a function pointer to a function which takes 2 ints as arguments and returns an int,
 *   - an int lhs
 *   - an int rhs
 *
 *   and returns the result of applying the function argument on lhs and rhs
 */

int add(int lhs, int rhs) {
    return lhs + rhs;
}

int mul(int lhs, int rhs) {
    return lhs * rhs;
}

namespace {
int apply_func(int (*f)(int, int), int lhs, int rhs) {
    return (*f)(lhs, rhs);
}
}    // namespace

#include <stdio.h>
int main() {
    int x = 2;
    int y = 3;
    int add_result = apply_func(&add, x, y);
    int mul_result = apply_func(&mul, x, y);
    printf("X+Y = %d\n", add_result);
    printf("X*Y = %d\n", mul_result);
}
