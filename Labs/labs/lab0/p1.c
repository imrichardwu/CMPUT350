#include <stdio.h>

// sort array A with n elements into non-decreasing order
// precond: n > 0
void simple_sort(int A[], int n) {
    for(int i = 0; i < n - 1; i++) { 
        for(int j = i + 1; j < n; j++) { 
            if (A[i] > A[j]){
                int temp = A[i];
                A[i] = A[j];
                A[j] = temp;
            }
        }
    }
}

// Test code
// check whether simple_sort works by setting up an array
// and printing its elements after sorting
void test() {
    int n = 5; 
    int A[5] = {3, 2, 4, 6, 5}; 
    simple_sort(A, n);
    for(int i = 0; i < n; i++) {
        printf("%d\n", A[i]);
    }
}