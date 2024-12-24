// g++ -std=c++17 -O3 nsieve.cpp -o nsieve
// /usr/bin/time --verbose ./nsieve 100000000
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

std::size_t nsieve(std::size_t n) {
    std::vector<bool> flags(n, false);
    std::size_t count = 0;
    for (std::size_t i = 2; i < n; ++i) {
        if (!flags[i]) {
            ++count;
            for (std::size_t j = i * 2; j < n; j += i) {
                flags[j] = true;
            }
        }
    }
    return count;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <n>\n";
        return 1;
    }

    auto n = static_cast<std::size_t>(std::stoi(argv[1]));
    auto c = nsieve(n);
    std::cout << "Number of primes up to n=" << n << ": " << c << std::endl;
}
