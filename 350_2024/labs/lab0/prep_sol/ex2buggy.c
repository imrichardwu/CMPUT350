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

    // deallocate(&m);
    //  we "forgot" to clean things up => memory leak
    //  => valgrind will report it at the end
    //  when running valgrind a.out

    return 0;
}
