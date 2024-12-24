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
    int i = 0;
    for (i = 0; i < n; i++) {    // i++ increments i by 1
        a[i] = a[i] + b[i];      // or a[i] += b[i]
    }
}

/*
Recall that arrays and pointers are indistinguishable in C when they
are passed on to functions as parameters:

- Pointers point to memory locations.

- Array parameters are pointers to the beginning of memory blocks
  storing elements.

So, above definition is equivalent to:

    void array_add(int *a, int *b, int n) {
        int i;
        for (i=0; i < n; i++) {
            a[i] = a[i] + b[i];
        }
    }


Issues:

- C-arrays don't know their size. So we have to pass on the length of arrays
  explicitely and trust the caller that the arrays actually have that size!

- The C runtime system does not offer checking out-of-bounds array access such
  as a[-1]. This can create large security problems (e.g. "buffer-overflow"
  attacks)

ERROR PRONE !

In C++ we can define classes that behave like arrays (including operators
such as [ ]), know their size, and are much safer.
*/

int main() {
    int u[N];    // allocates int arrays of length N on stack
    int v[N];

    // adds v[i] to u[i] for i=0..N-1
    array_add(u, v, N);
    return 0;
}
