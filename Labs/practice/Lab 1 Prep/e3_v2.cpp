#include <iostream>

// Cleans up allocated memory and exists with proper return value
int exit_program(int A[], int ret_val) {
    delete[] A;
    return ret_val;
}

int main() {
    // Constants
    constexpr int MAX = 10000000;
    constexpr int ERR_VAL = 1;
    constexpr int OK_VAL = 0;

    int *A = new int[MAX];
    int count = 0;

    // read data and append
    while (true) {
        int input = 0;
        std::cin >> input;

        // Input couldn't be read (either eof or error)
        if (!std::cin) {
            break;
        }

        // Check for too many inputs
        if (count >= MAX) {
            std::cerr << "input too big." << std::endl;
            return exit_program(A, ERR_VAL);
        }

        // Input good...
        A[count++] = input;
    }

    // No longer reading input because either eof or error ..
    // If error ...
    if (!std::cin.eof()) {
        std::cerr << "illegal input" << std::endl;
        return exit_program(A, ERR_VAL);
    }

    // OK: Print in reverse order
    while (--count >= 0) {
        std::cout << A[count] << " ";
    }
    std::cout << std::endl;
    return exit_program(A, OK_VAL);
}
