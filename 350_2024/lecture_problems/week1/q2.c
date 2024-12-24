// Question
/**
 * How do you get length of an array in C?
 */

// Solution
#include <stdio.h>
int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int arr_size = sizeof(arr) / sizeof(arr[0]);
    printf("arr has %d elements.\n", arr_size);
}
