#include <stdio.h>
#include <stdlib.h>

// C99 understands const ... so use it as much as possible

// a point on the integer grid
struct Point {
    // data
    int x, y;
};

// constructor
void Point_constr(struct Point *this) {
    this->x = this->y = 0;
}

// default destructor (written by compiler)
void Point_destr(struct Point *this) {
    // to make compiler happy (otherwise it complains about parameter
    // not being used)
    // Note: in C++ one can simply omit the parameter name if a parameter
    // isn't used
    this = this;
}

// add point componentwise
void Point_add(struct Point *this, const struct Point *p) {
    this->x += p->x;
    this->y += p->y;
}

// print to standard output
// note: const function => make this a pointer to const struct Point
void Point_print(const struct Point *this) {
    printf("[%d,%d]\n", this->x, this->y);
}

int main() {
    // was: const int N = 200;
    const int N = 200;

    // was: Point *A = new Point[N];
    struct Point *A = malloc(N * sizeof(struct Point));

    int i;
    for (i = 0; i < N; ++i) {
        Point_constr(&A[i]);
    }

    // was: Point sum;
    struct Point sum;
    Point_constr(&sum);

    // was: for (int i=0; i < N; ++i) { A[i].x = i; A[i].y = -i; sum.add(A[i]); }
    for (i = 0; i < N; ++i) {
        A[i].x = i;
        A[i].y = -i;
        Point_add(&sum, &A[i]);
    }

    // was: sum.print();
    Point_print(&sum);

    // was: delete[] A;
    // destroy elements in reverse order
    // [ a good optimizing compiler will not generate this code because the
    // destructor doesn't do anything ]
    for (i = N - 1; i >= 0; --i) {
        Point_destr(&A[i]);
    }
    free(A);

    // sum goes out of scope => destroy
    Point_destr(&sum);

    return 0;
}
