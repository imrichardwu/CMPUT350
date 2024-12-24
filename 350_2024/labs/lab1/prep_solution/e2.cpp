#include <iostream>

// returns 0 if input OK, 1 otherwise
int main() {
    // partial sum of read integers
    int sum = 0;
    int input{};

    while (std::cin >> input) {
        sum += input;
    }

    if (std::cin.eof()) {
        // eof - print sum
        std::cout << sum << std::endl;
        return 0;

    } else {
        // error
        std::cerr << "illegal input" << std::endl;
        return 1;
    }
}
