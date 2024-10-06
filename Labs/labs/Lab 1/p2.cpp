/*
  Write C++ program p2.cpp that reads a sequence of int array pairs from stdin
  and prints their component-wise sum to stdout.
*/

#include <cassert>
#include <cstdlib>
#include <iostream>

// helper function:
// free array, print error message to stderr, and
// exit program with value 1 - indicating an error
void error(int *A) {
    delete[] A;
    std::cerr << "input error" << std::endl;
    exit(1);
}

// solve problem instance (array size n)
void solve_instance(int n) {
    // Invalid instance size
    if (n <= 0) {
        error(nullptr);
    }

    //  allocate and read first array
    int *A = new int[static_cast<unsigned int>(n)];
    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    // did any error occur when reading the first array?
    if (!std::cin) {
        error(A);
    }

    std::cout << n << std::endl;

    // read second array and print component-wise sum
    for (int i = 0; i < n; ++i) {
        int b;
        if (!(std::cin >> b)) {
            error(A);
        }
        std::cout << (b * A[i]) << " ";
    }

    std::cout << std::endl;

    // clean up 
    delete[] A;
}

int main() {
    int n;

    while (std::cin >> n) {
        // handle one input instance
        //  n = read number of array elements
        solve_instance(n);
    }

    // If not EOF then input doesn't match the specification
    if (!std::cin.eof()) {
        error(nullptr);
    }

    return 0;
}
