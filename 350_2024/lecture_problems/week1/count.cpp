// g++ -std=c++17 -O3 count.cpp -o count
// /usr/bin/time --verbose ./count 1000000000
#include <cstddef>
#include <iostream>
#include <string>

std::size_t count(std::size_t n) {
    std::size_t count = 0;
    for (std::size_t i = 0; i < n; ++i) {
        ++count;
    }
    return count;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <n>\n";
        return 1;
    }

    auto n = static_cast<std::size_t>(std::stoi(argv[1]));
    auto c = count(n);
    std::cout << "Number of numbers up to n=" << n << ": " << c << std::endl;
}
