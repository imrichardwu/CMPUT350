#include <iostream>
#include <map>
#include <boost/multiprecision/cpp_int.hpp>

long long choose(int n, int k) {
    if (k == 0 || k == n) {
        return 1;
    }
    return choose(n - 1, k - 1) + choose(n - 1, k);
}

// long long choose_m(int n, int k) {
//     static std::map<std::pair<int, int>, long long> cache;
//     if (k == 0 || k == n) {
//         return 1;
//     }
//     std::pair<int, int> key(n, k);
//     if (cache.find(key) != cache.end()) {
//         return cache[key];
//     }
//     cache[key] = choose_m(n - 1, k - 1) + choose_m(n - 1, k);
//     return cache[key];
// }

boost::multiprecision::cpp_int choose_m(int n, int k) {
    static std::map<std::pair<int, int>, boost::multiprecision::cpp_int> cache;
    if (k == 0 || k == n) {
        return 1;
    }
    std::pair<int, int> key(n, k);
    if (cache.find(key) != cache.end()) {
        return cache[key];
    }
    cache[key] = choose_m(n - 1, k - 1) + choose_m(n - 1, k);
    return cache[key];
}

int main() {
    int n = 1000; // Example value
    int k = n;
    
    long long result = choose(2*n, k);
    std::cout << "choose(" << n << ", " << k << ") = " << result << std::endl;
    return 0;
}
