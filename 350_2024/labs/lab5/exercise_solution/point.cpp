#include <cstddef>
#include <iostream>

using namespace std;

//  simple 2d point structure
template <typename T>
struct Point {
    T x, y;

    Point(const T &x0 = T{}, const T &y0 = T{})
        : x(x0), y(y0) {}

    //  print point to stdout
    void print() const {
        cout << "(" << x << "," << y << ")";
    }

    // return true iff components are equal
    bool operator==(const Point &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

//  return the sum of n points
template <typename T>
Point<T> sum(const Point<T> *A, int n) {
    Point<T> sum;
    for (int i = 0; i < n; ++i) {
        sum.x += A[i].x;
        sum.y += A[i].y;
    }
    return sum;
}

//  return smallest index of p in array
//  or -1 if not found
template <typename T>
int find(const Point<T> *A, int n, const Point<T> &p) {
    for (int i = 0; i < n; ++i) {
        if (A[i] == p) {
            return i;
        }
    }
    return -1;
}

//  test helper function template
//  expects reference to C-array of T, deduces its length n
//  prints array and sum and compares it with expected value
template <typename T, size_t n>
void sum_test(bool &ok, Point<T> (&A)[n], const Point<T> &expected) {
    Point<T> r = sum(A, n);

    for (int i = 0; i < static_cast<int>(n); ++i) {
        A[i].print();
        cout << ' ';
    }

    bool ok_sum = (r == expected);
    cout << ": ";
    r.print();
    cout << " OK=" << ok_sum << endl;
    ok &= ok_sum;
}

//  test helper function template
//  expects reference to C-array of T, deduces its length n, and point p
//  prints array, p, and index and compares it with expected value
template <typename T, size_t n>
void find_test(bool &ok, Point<T> (&A)[n], const Point<T> &p, int expected) {
    int r = find(A, n, p);

    for (int i = 0; i < static_cast<int>(n); ++i) {
        A[i].print();
        cout << ' ';
    }
    bool ok_index = (r == expected);
    cout << ": " << r;
    cout << " OK=" << ok_index << endl;
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
        cout << "all tests passed" << endl;
        return 0;
    } else {
        cout << "at least one test failed!" << endl;
        return 1;
    }

    return 0;
}
