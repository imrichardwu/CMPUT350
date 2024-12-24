#include <stdlib.h>    // declares malloc/free

struct Matrix {
    int rows;
    int cols;
    double *a;    // pointer to rows*cols elements
};

// initialize matrix pointed to by m with r rows and c columns
void init(struct Matrix *m, int r, int c) {
    m->rows = r;    // access structure element; equivalent to (*m).rows = r
    m->cols = c;

    // allocate enough memory for r * c doubles and set it to 0
    // binary represenation of double 0.0 is 8 zeroes...
    // sizeof(T) = number of bytes a variable of type T occupies in memory
    m->a = calloc((size_t)(r * c), sizeof(m->a[0]));
}

// free memory associated with matrix pointed to by m
void deallocate(struct Matrix *m) {
    free(m->a);    // return memory to the operating system
}

int main() {
    struct Matrix m;

    init(&m, 20, 30);    // pass address of m and dimensions to function
    // ... use matrix m
    deallocate(&m);
    return 0;
}

/*
Issue:

What happens if we forget to call deallocate() ?

If this happens in a loop, we keep on allocating memory until there is
none available anymore and the program crashes.

Memory leaks are deadly for long running server applications.

C++ provides automatic initialization and deallocation whenever
variables go out of scope:


For instance, in C++ we could write

    int main() {
        ...

        if (...) {

            Matrix m(20, 30); // create 20 x 30 matrix, no need for prefix "struct"

            // ... use m ...

            // Objects that go out of scope are destroyed automatically.
            // At this point memory bound to m is released by calling the
            // Matrix destructor code which the author of Matrix provides.
            // No need to call deallocate explicitly!
        }
        return 0;
    }
*/
