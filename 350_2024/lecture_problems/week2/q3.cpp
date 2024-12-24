// Question
/**
 * Write a C++ code snippet that on the memory heap first allocates
 * an aray of N pointers to arrays containing M ints each,
 * and then frees all allocated memory -- using new/delete
 */

// Solution
int main() {
    const int N = 20;
    const int M = 30;

    // allocate array of pointers of length N
    // A is a pointer to an array of ints
    int **A = new int *[N];

    // note: in C/C++ pointers and arrays are (almost) identical types. So, int
    // *A could be a pointer to a single int, or the starting address of an int
    // array, as B in:
    //   int A[10], *B = A;
    // is B pointing to a single int or an array?

    for (int i = 0; i < N; ++i) {
        // each pointer points to int array of length M
        A[i] = new int[M];    // NOLINT(*-pointer-arithmetic,*-owning-memory)
    }

    // A is essentially an N by M matrix

    A[2][3] = 5;    // sets element at row 2 and col 3 to 5

    // @NOTE: explain step by step what happens in above line

    // @NOTE: free matrix memory in reverse order (why?)

    for (int i = 0; i < N; ++i) {
        delete[] A[i];    // matching operators: new[] requires delete[]
    }

    delete[] A;

    return 0;
}
