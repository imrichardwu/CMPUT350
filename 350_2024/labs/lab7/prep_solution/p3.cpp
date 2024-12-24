#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <utility>

// boost 1.53+ needed
// compute exact results using boost::multiprecision variables
#define MP_TEST 1

#ifdef MP_TEST
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
#endif

using namespace std;

using LL = long long;

// slow recursion
LL choose(int n, int k) {
    assert(k >= 0 && k <= n);
    if (k == 0 || k == n) {
        return 1;
    }

    // choose(n, k) = choose(n, n-k)
    // => faster
    if (k > n - k) {
        k = n - k;
    }

    return choose(n - 1, k - 1) + choose(n - 1, k);
}

// memoized version
LL choose_m(int n, int k) {
    // careful! this makes the function non-reentrant
    static map<pair<int, int>, LL> memo;

    assert(k >= 0 && k <= n);
    if (k == 0 || k == n) {
        return 1;
    }

    // choose(n, k) = choose(n, n-k)
    if (k > n - k) {
        k = n - k;
    }
    pair<int, int> p(n, k);
    auto it = memo.find(p);

    if (it != memo.end()) {
        return it->second;
    }

    LL v = choose_m(n - 1, k - 1) + choose_m(n - 1, k);
    memo[p] = v;    // insert result v = choose(n, k) into map
    return v;
}

// iterative version        n!        n *(n-1)*...*(n-k+1)
// using choose(n, k) =  --------   = -------------------
//                       k! (n-k)!    1 * 2   *...* k
// where
//   n! ("n factorial") is defined like this:
//   0! = 1
//   n! = n (n-1)!, for n > 0
//
// Start with 1, then multiply with n/1, followed by multiplying with (n-1)/2,
// etc. Note that each sub-product is an integer, because it represents
// choose(n,l) for some 1 <= l <= k !

LL choose_i(int n, int k) {
    assert(k >= 0 && k <= n);

    // choose(n, k) = choose(n, n-k), pick smaller second argument
    if (k > n - k) {
        k = n - k;
    }
    LL ans = 1;
    for (int x = 0; x < k; ++x) {
        ans = (ans * (n - x)) / (x + 1);    // always divisible
    }
    return ans;
}

// problem: intermediate results in choose_i can be larger than result
// solution: divide by gcd

// computes greatest common divisor (gcd) of a, b >= 0 using Euclid's
// algorithm. E.g., gcd(9,12) = 3, gcd(0,k) = k
int gcd(int a, int b) {
    assert(a >= 0 && b >= 0);
    while (a) {
        int c = a;
        a = b % a;
        b = c;
    }
    return b;
}

// fewer overflows ...
LL choose_i2(int n, int k) {
    assert(k >= 0 && k <= n);

    // choose(n, k) = choose(n, n-k), pick smaller second argument
    if (k > n - k) {
        k = n - k;
    }
    LL ans = 1;
    for (int x = 0; x < k; ++x) {
        int a = n - x;
        int b = x + 1;
        int g = gcd(a, b);
        if (g != 1) {
            a /= g;
            b /= g;
            // answer must now be divisible by g (otherwise, a,b would
            // still have some prime factor in common)
        }
        ans = (ans / b) * a;
    }
    return ans;
}

#if MP_TEST

// boost multi-precision version
// cpp_int = arbitrary precision int
cpp_int choose_mp(int n, int k) {
    assert(k >= 0 && k <= n);

    // choose(n, k) = choose(n, n-k), pick smaller second argument
    if (k > n - k) {
        k = n - k;
    }
    cpp_int ans = 1;
    for (int x = 0; x < k; ++x) {
        ans = (ans * (n - x)) / (x + 1);    // always divisible
    }
    return ans;
}
#endif

int main() {
    int N = 34, K = 17;
    // cout << choose(N, K) << endl;   // 7.9 sec
    // cout << choose_m(N, K) << endl;   // 0.001 sec
    cout << choose_i(N, K) << endl;    // 0.001 sec

    cout << "mp m i i2" << endl;
    for (int i = 1; i < 60; ++i) {
        int n = 2 * i;
        int k = i;
        cout << n << " " << k << " : "
#if MP_TEST
             << choose_mp(n, k) << " "    // true value
#endif
             << choose_m(n, k) << " "    // memoized
             << choose_i(n, k) << " "    // iterative
             << choose_i2(n, k)          // iterative, delayed overflow
             << endl;
    }

    cpp_int i("100000000000000000000000000");
    cpp_int j = 2;
    cout << i * j << endl;
    return 0;
}

/*
  2 1 : 2 2 2 2
  4 2 : 6 6 6 6
  6 3 : 20 20 20 20
  8 4 : 70 70 70 70
  10 5 : 252 252 252 252
  12 6 : 924 924 924 924
  14 7 : 3432 3432 3432 3432
  16 8 : 12870 12870 12870 12870
  18 9 : 48620 48620 48620 48620
  20 10 : 184756 184756 184756 184756
  22 11 : 705432 705432 705432 705432
  24 12 : 2704156 2704156 2704156 2704156
  26 13 : 10400600 10400600 10400600 10400600
  28 14 : 40116600 40116600 40116600 40116600
  30 15 : 155117520 155117520 155117520 155117520
  32 16 : 601080390 601080390 601080390 601080390
  34 17 : 2333606220 2333606220 2333606220 2333606220
  36 18 : 9075135300 9075135300 9075135300 9075135300
  38 19 : 35345263800 35345263800 35345263800 35345263800
  40 20 : 137846528820 137846528820 137846528820 137846528820
  42 21 : 538257874440 538257874440 538257874440 538257874440
  44 22 : 2104098963720 2104098963720 2104098963720 2104098963720
  46 23 : 8233430727600 8233430727600 8233430727600 8233430727600
  48 24 : 32247603683100 32247603683100 32247603683100 32247603683100
  50 25 : 126410606437752 126410606437752 126410606437752 126410606437752
  52 26 : 495918532948104 495918532948104 495918532948104 495918532948104
  54 27 : 1946939425648112 1946939425648112 1946939425648112 1946939425648112
  56 28 : 7648690600760440 7648690600760440 7648690600760440 7648690600760440
  58 29 : 30067266499541040 30067266499541040 30067266499541040 30067266499541040
  60 30 : 118264581564861424 118264581564861424 118264581564861424 118264581564861424  ^ALL OK
  62 31 : 465428353255261088 465428353255261088 -284401161134521734 465428353255261088    choose_i wrong
  64 32 : 1832624140942590534 1832624140942590534 -275878929539273115 1832624140942590534  choose_i wrong
  66 33 : 7219428434016265740 7219428434016265740 274431684008374997 7219428434016265740     choose_i wrong
  68 34 : 28453041475240576740 -8440446672178526492 -2638506530098427 9011912633654808675      all wrong
*/
