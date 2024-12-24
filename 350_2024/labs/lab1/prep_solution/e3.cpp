#include <iostream>

int main() {
    const int ERR_VAL = 1;    // 0: ok
    const int MAX = 10000000;

    // allocate MAX ints on the heap
    // important: int A[MAX]; doesn't work
    // because stack is too small
    int *A = new int[MAX];
    int count = 0;

    // read data into array
    for (;;) {
        int input;

        std::cin >> input;
        if (!std::cin) break;

        if (count >= MAX) {
            std::cerr << "input too big" << std::endl;
            delete[] A;
            return ERR_VAL;
        }

        // input OK => store it
        A[count++] = input;
    }

    int ret_val = 0;

    if (std::cin.eof()) {
        // input OK: print in reverse
        while (--count >= 0) {
            std::cout << A[count] << " ";
        }
        std::cout << std::endl;

    } else {
        // otherwise: error
        std::cerr << "illegal input\n";
        ret_val = ERR_VAL;
    }

    // free data before exiting
    delete[] A;
    return ret_val;
}
