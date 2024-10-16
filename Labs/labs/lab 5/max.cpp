#include <iostream>
#include <cassert>
#include <cstring>

size_t index_of_max(const int *A, size_t n) {
    assert(n != 0); // Checl if array is empty
    size_t max_index = 0;
    for (size_t i = 1; i < n; ++i) {
        if (A[max_index] < A[i]) {   // Loop through array to find max element index
            max_index = i;
        }
    }
    return max_index;
}

template <typename T>
size_t index_of_max(const T *A, size_t n) {
    assert(n != 0); // Checl if array is empty
    size_t max_index = 0;
    for (size_t i = 1; i < n; ++i) {
        if (A[max_index] < A[i]) {  // Loop through array to find max element index
            max_index = i;
        }
    }
    return max_index;
}

// Overload for const char*
size_t index_of_max(const char* *A, size_t n) {
    assert(n != 0); // Checl if array is empty
    size_t max_index = 0;
    for (size_t i = 1; i < n; ++i) {
        if (strcmp(A[max_index], A[i]) < 0) {  // Using strcmp for lex order
            max_index = i;
        }
    }
    return max_index;
}

// Overload for char*
size_t index_of_max(char** A, size_t n) {
    assert(n != 0); // Checl if array is empty
    size_t max_index = 0;
    for (size_t i = 1; i < n; ++i) {
         if (strcmp(const_cast<const char*>(A[max_index]), const_cast<const char*>(A[i])) < 0) {
            max_index = i;
        }
    }
    return max_index;
}

void print(const int *a, size_t n) {
    for(size_t i = 0; i < n; i++) { // Loop thorugh to print
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void print(const T *a, size_t n) {
    for(size_t i = 0; i < n; i++) { // Loop thorugh to print
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int a[] = {1, 4, 8, 7, 5};
    print(a, 5);
    std::cout << "Index of max: " << index_of_max(a, 5) << std::endl;

    double b[] = {4.0, 4.2, 3.7, 4.4, 8.5};
    print(b, 5);
    std::cout << "Index of max: " << index_of_max(b, 5) << std::endl;

    const char* c[] = {"a", "j", "n", "k", "d"};
    print(c, 5);
    std::cout << "Index of max: " << index_of_max(c, 5) << std::endl;

    char* d[] = {"a", "b", "x", "y", "k"};
    print(d, 5);
    std::cout << "Index of max: " << index_of_max(d, 5) << std::endl;
    return 0;
}