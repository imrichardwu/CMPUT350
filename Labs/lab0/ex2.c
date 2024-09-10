#include <stdio.h>
#include <stdlib.h>  // Include the required header for calloc, exit, and free

struct Matrix {
    int rows;
    int cols;
    double *a; // pointer to rows*cols elements
};

// initialize matrix pointed to by m with r rows and c columns
// i.e. allocate sufficient memory and set all elements to 0
void init(struct Matrix *m, int r, int c) {
    m->rows = r;  
    m->cols = c;   
    m->a = (double *)calloc(r * c, sizeof(double));  // Allocate memory for r*c elements
    if (m->a == NULL) {  // Check if memory allocation failed
        fprintf(stderr, "Memory allocation failed\n");  
        exit(1);  // Exit the program if memory allocation failed
    }
}

// free memory associated with matrix pointed to by m
void deallocate(struct Matrix *m) {
    free(m->a);  // Free the dynamically allocated memory
}

int main() {
    struct Matrix m;
    init(&m, 20, 30); // pass address of m and dimensions to function
    // ... use matrix m
    deallocate(&m);   // free memory
    return 0;
}
