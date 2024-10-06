#include <stdio.h>
#include <stdlib.h>

struct Point {
    int x, y;
};

// initialize x and y to zero
void init(struct Point *p) {
    p->x = 0;
    p-> y = 0;
}

// add two points componentwise
void add(struct Point *p, const struct Point *q) {
    p->x += q->x;
    p->y += q->y;
}

// print the point to standard output
void print(const struct Point *p) {
    printf("[%d, %d]\n", p->x, p->y);
}

int main() {
    const int N = 200;

    // allocate an array of N points
    struct Point *A = (struct Point*)malloc(N * sizeof(struct Point));

    // allocate a point to store the sum
    struct Point sum;
    // initialize sum to zero
    init(&sum);

    // initialize each A element to a different value
    for(int i = 0; i < N; i++){
        A[i].x = i;
        A[i].y = -i;
        add(&sum, &A[i]);
    }

    print(&sum);
    free(A);
    return 0;
}