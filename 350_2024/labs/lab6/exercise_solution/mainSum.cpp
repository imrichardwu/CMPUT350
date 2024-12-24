#include <iostream>

using namespace std;

/*
  sum(n) = 1+...+n
  sum(0) = 0

  => sum(n) = n + sum(n-1) for n > 0
*/

// recursion step
template <int N>
struct Sum {
    static_assert(N > 0, "N must be > 0");
    enum {
        value = N + Sum<N - 1>::value
    };
};

// base case
template <>
struct Sum<0> {
    enum {
        value = 0
    };
};

// simpler
template <int N>
struct Sum2 {
    static_assert(N >= 0, "N must be >= 0");
    enum {
        value = N * (N + 1) / 2
    };
};

int main() {
    // cout << Sum<-1>::value  << endl;  // prints 0

    cout << Sum<0>::value << endl;      // prints 0
    cout << Sum<10>::value << endl;     // prints 55
    cout << Sum<20>::value << endl;     // prints 210
    cout << Sum<100>::value << endl;    // prints 5050

    cout << endl;

    cout << Sum2<0>::value << endl;      // prints 0
    cout << Sum2<10>::value << endl;     // prints 55
    cout << Sum2<20>::value << endl;     // prints 210
    cout << Sum2<100>::value << endl;    // prints 5050

    // cout << Sum2<-1>::value << endl; // illegal, caught by static_assert
    return 0;
}
