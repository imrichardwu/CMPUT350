#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Include students submission
#include "p1.h"

void simple_sort_solution(int A[], int n) {
    assert(n > 0);
    for (int i = 0; i < n; ++i) {
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

// Free memory and exit
void cleanup(int *A, int *A_sol, int exit_code) {
    free(A);
    free(A_sol);
    exit(exit_code);
}

// Run for an array of inputs
int main(int argc, char *argv[]) {
    (void)argc;
    if (argc <= 1) {
        printf("Error: need to run with command line argument of array size.\n");
        exit(1);
    }
    int N = atoi(argv[1]);

    if (N <= 0) {
        printf("Expecting program to hit assert.\n");
        simple_sort(NULL, 0);
    } else {
        // Create random array
        int *A = (int *)malloc((unsigned long)N * sizeof(int));
        int *A_sol = (int *)malloc((unsigned long)N * sizeof(int));
        for (int i = 0; i < N; ++i) {
            A[i] = rand() % 100;
            A_sol[i] = A[i];
        }

        simple_sort_solution(A_sol, N);
        simple_sort(A, N);

        // Compare
        for (int i = 0; i < N; ++i) {
            if (A[i] != A_sol[i]) {
                printf("Failed.\n");
                cleanup(A, A_sol, 1);
            }
        }
        printf("Success.\n");
        cleanup(A, A_sol, 0);
    }
}
