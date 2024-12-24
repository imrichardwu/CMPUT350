#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

//  T=int function
size_t index_of_max(const int *A, size_t n) {
    assert(n > 0);
    size_t imax = 0;

    for (size_t i = 1; i < n; ++i) {
        if (A[imax] < A[i]) {
            imax = i;
        }
    }
    return imax;
}

//  default function template
template <typename T>
size_t index_of_max(const T *A, size_t n) {
    assert(n > 0);
    size_t imax = 0;

    for (size_t i = 1; i < n; ++i) {
        if (A[imax] < A[i]) {
            imax = i;
        }
    }
    return imax;
}

//  T=const char* function template
size_t index_of_max(const char **A, size_t n) {
    assert(n > 0);
    size_t imax = 0;

    for (size_t i = 1; i < n; ++i) {
        if (strcmp(A[imax], A[i]) < 0) {
            imax = i;
        }
    }
    return imax;
}

//  T=char* function template
size_t index_of_max(char **A, size_t n) {
    assert(n > 0);
    size_t imax = 0;

    for (size_t i = 1; i < n; ++i) {
        if (strcmp(A[imax], A[i]) < 0) {
            imax = i;
        }
    }
    return imax;
}

//  print int array
void print(const int *A, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        cout << A[i] << ' ';
    }
}

//  print array of element type T
template <typename T>
void print(const T *A, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        cout << A[i] << ' ';
    }
}

//  test helper function template
//  expects reference to C-array of T, deduces its length n
//  prints array and return value of index_of_max and compares it with expected
//  value
template <typename T, size_t n>
void test(bool &ok, T (&A)[n], size_t expected) {
    size_t r = index_of_max(A, n);
    print(A, n);
    bool ok_loc = (r == expected);
    cout << " : " << r << " OK=" << ok_loc << endl;
    ok &= ok_loc;
}

// return 0 if all test past, 1 otherwise

int main() {
    bool ok = true;

    {    // test const c-strings
        const char *A[] = {"aaa", "fff", "bbb", "fff", "ccc"};
        test(ok, A, 1);
    }

    {    // test non-const c-strings
        char *A[] = {
            const_cast<char *>("aaa"),
            const_cast<char *>("fff"),
            const_cast<char *>("bbb"),
            const_cast<char *>("fff"),
            const_cast<char *>("ccc")
        };
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
        cout << "all tests passed" << endl;
        return 0;
    } else {
        cout << "at least one test failed!" << endl;
        return 1;
    }
}
