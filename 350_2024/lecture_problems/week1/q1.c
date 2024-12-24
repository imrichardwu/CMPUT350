// Question
/**
 * What is the difference between:
 *   int *
 *   const int *
 *   int const *
 *   int * const
 *   const int * const
 */

// Solution
int main() {
    int x = 10;
    int y = 20;

    const int *ptr_to_cnst = &x;
    *ptr_to_cnst = 10;    // ERROR
    ptr_to_cnst = &y;     // OK

    int * const cnst_pointer = &x;
    *cnst_pointer = y;    // OK
    cnst_pointer = &y;    // ERROR

    return 0;
}
