#include <stdio.h>

// returns 0 if input OK, 1 otherwise
int main() {
    // partial sum of read integers
    int sum = 0;
    int input = 0;

    while (scanf("%d", &input) == 1) {
        sum += input;
    }

    if (feof(stdin)) {
        // eof - print sum
        printf("%d\n", sum);
        return 0;

    } else {
        // error
        fprintf(stderr, "illegal input\n");
        return 1;
    }
}
