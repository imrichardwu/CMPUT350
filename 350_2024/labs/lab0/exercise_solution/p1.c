#include <assert.h>
#include <stdio.h>

// sort array A with n elements
// into non-decreasing order 
// precond: n > 0
void simple_sort(int A[], int n) {
    assert(n > 0);
    int i = 0;
    for (i = 0; i < n; ++i) {
        // find minimum element in A[0..i] 
        int min_j = i;
        int j = 0;
        for (j = i + 1; j < n; ++j) {
            if (A[j] < A[min_j]) {
                min_j = j;
            }
        }
        // swap A[i] and A[min_j] 
        int t = A[i];
        A[i] = A[min_j];
        A[min_j] = t;
    }
}

// Test code
// check whether simple_sort works by setting up an array
// and printing its elements after sorting
void test() {
    // test code
    const int N = 10;
    int A[N];

    // populate array with N-1..0
    int i = 0;
    for (i = 0; i < N; ++i) {
        A[i] = N - 1 - i;
    }

    // print it
    printf("Original array:\n");
    for (i = 0; i < N; ++i) {
        printf("%d %d\n", i, A[i]);
    }

    // sort it
    simple_sort(A, N);

    // print it
    printf("Sorted array:\n");
    for (i = 0; i < N; ++i) {
        printf("%d %d\n", i, A[i]);
    }

    // should be: 0 ... N-1
}
