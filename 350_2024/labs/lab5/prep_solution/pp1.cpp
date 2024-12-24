#include <iostream>

using namespace std;

// swap values if first is bigger (only using <)
template <typename T>
void swap_if_bigger(T &a, T &b) {
    if (b < a) {
        T t(b);
        b = a;
        a = t;
        // or: std::swap(a, b)
    }
}

// test helper function (only using <)
template <typename T>
void test(bool &ok, const T &a0, const T &b0) {
    cout << "test " << a0 << " " << b0;

    T a(a0), b(b0);

    swap_if_bigger(a, b);

    bool ok_loc = (b0 < a0) ? ((a0 == b) && (b0 == a)) : ((a0 == a) && (b0 == b));
    ok &= ok_loc;
    cout << " -> " << a << " " << b << " OK=" << ok_loc << endl;
}

int main() {
    bool ok = true;

    {    // int test
        int a = 2, b = 1;
        test(ok, a, b);
    }
    {    // double test
        double a = 0.5, b = 1.3;
        test(ok, a, b);
    }

    if (ok) {
        cout << "all tests past" << endl;
        return 0;
    } else {
        cout << "at least one test failed!" << endl;
        return 1;
    }
}
