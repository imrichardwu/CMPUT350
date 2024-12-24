#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>

#define main YY_main
#define test YY_test
#include "max.cpp"
#undef main
#undef test

//  test helper function template
//  expects reference to C-array of T, deduces its length n
//  prints array and return value of index_of_max and compares it with expected
//  value
template <typename T, std::size_t n>
void test(bool &ok, T (&A)[n], std::size_t expected) {
    std::size_t r = index_of_max(A, n);
    print(A, n);
    bool ok_loc = (r == expected);
    std::cout << " : " << r << " OK=" << ok_loc << std::endl;
    ok &= ok_loc;
}

int main() {
    bool ok = true;

    {    // test const c-strings
        const char *A[] = {"aaa", "fff", "bbb", "fff", "ccc"};
        test(ok, A, 1);
    }

    {    // test non-const c-strings
        char *A[] = {(char *)"aaa", (char *)"fff", (char *)"bbb", (char *)"fff", (char *)"ccc"};
        test(ok, A, 1);
    }

    {    // test int
        int A[] = {1, 2, 5};
        test(ok, A, 2);
    }

    {    // test double
        double A[] = {1.0, 2.0, 5.0, 6.0, 5.0};
        test(ok, A, 3);
    }

    if (ok) {
        std::cout << "all tests passed" << std::endl;
        return 0;
    } else {
        std::cout << "at least one test failed!" << std::endl;
        return 1;
    }
}
