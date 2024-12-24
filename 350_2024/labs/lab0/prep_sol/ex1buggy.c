// C-style constant: N gets replace by 20 below
// (deprecated in C++)
#define N 20

/*
  input:
  a,b: arrays of n integers

  effect:
  add b to a element-wise
*/

void array_add(int a[], int b[], int n) {
    int i;
    for (i = 0; i < n; i++) {    // i++ increments i by 1
        a[i] = a[i] + b[i];      // or a[i] += b[i]
    }
}

int main() {
    int u[N];    // allocates int arrays of length N on stack
    int v[N];

    int *p = 0;
    *p = 9;    // accesses system memory => "segmention fault" fffffffff

    // adds v[i] to u[i] for i=0..N-1
    array_add(u, v, N);
    return 0;
}
