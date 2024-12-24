#include <cstdlib>
#include <iostream>

int factorial(int n) {
    int result = 1;
    while (n > 1) {
        result *= n;
        --n;
    }
    return result;
}

int main(int argc, char **argv) {
    int n = 13;
    if (argc >= 2) {
        n = std::atoi(argv[1]);
    }
    std::cout << "Computing " << n << "!" << std::endl;
    std::cout << factorial(n) << std::endl;
}
