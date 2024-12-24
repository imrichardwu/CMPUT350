#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>

#define main      YY_main
#define find_test YY_find_test
#define sum_test  YY_sum_test
#include "point.cpp"
#undef main
#undef find_test
#undef sum_test

//  test helper function template
//  expects reference to C-array of T, deduces its length n
//  prints array and sum and compares it with expected value
template <typename T, std::size_t n>
void sum_test(bool &ok, Point<T> (&A)[n], const Point<T> &expected) {
    Point<T> r = sum(A, n);

    for (int i = 0; i < static_cast<int>(n); ++i) {
        A[i].print();
        std::cout << ' ';
    }

    bool ok_sum = (r == expected);
    std::cout << ": ";
    r.print();
    std::cout << " OK=" << ok_sum << std::endl;
    ok &= ok_sum;
}

//  test helper function template
//  expects reference to C-array of T, deduces its length n, and point p
//  prints array, p, and index and compares it with expected value
template <typename T, std::size_t n>
void find_test(bool &ok, Point<T> (&A)[n], const Point<T> &p, int expected) {
    int r = find(A, n, p);

    for (int i = 0; i < static_cast<int>(n); ++i) {
        A[i].print();
        std::cout << ' ';
    }
    bool ok_index = (r == expected);
    std::cout << ": " << r;
    std::cout << " OK=" << ok_index << std::endl;
    ok &= ok_index;
}

int main() {
    bool ok = true;

    // test sum

    {    //  test double Points
        using T = Point<double>;
        T A[] = {T(1, 2.5), T(3.25, 4)};
        sum_test(ok, A, T(4.25, 6.5));
    }

    {    //  test int Points
        using T = Point<int>;
        T A[] = {T(1, 2), T(3, 4)};
        sum_test(ok, A, T(4, 6));
    }

    // test find

    {    //  test double Points

        // found
        using T = Point<double>;
        T A[] = {T(1, 2.5), T(3.25, 4)};
        T p(3.25, 4);
        find_test(ok, A, p, 1);
    }

    {    //  test int Points

        // not found
        using T = Point<double>;
        T A[] = {T(2, 3), T(4, 5)};
        T p(5, 6);
        find_test(ok, A, p, -1);
    }

    if (ok) {
        std::cout << "all tests passed" << std::endl;
        return 0;
    } else {
        std::cout << "at least one test failed!" << std::endl;
        return 1;
    }

    return 0;
}
