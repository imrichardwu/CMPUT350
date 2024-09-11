#define N 20 // C-style constant: N gets replaced by 20 below

void array_add(int *u, int *v, int n) {
    for (int i = 0; i < n; i++) {
        u[i] += v[i];
    }
}

int main() {
    // At this point, arrays 'u' and 'v' are declared but NOT initialized.
    // This means they contain random "garbage" values.

    int u[N]; // allocates int arrays of length N on stack
    int v[N];

    // Calls the function to add corresponding elements of v to u
    // Since both arrays contain uninitialized values, this operation works with garbage values.
    int *p = 0;
    *p = 0;

    // adds v[i] to u[i] for i=0..N-1
    array_add(u, v, N);
    return 0;
}