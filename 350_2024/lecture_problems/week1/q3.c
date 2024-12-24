// Question
/**
 * Implement a memcpy to copy a source string to a destination string.
 */

// Solution
#include <stdio.h>

void *custom_memcpy(void *dest, const void *src, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        ((char *)dest)[i] = ((const char *)src)[i];
    }
    return dest;
}

int main() {
    char src[] = "Hello There!";
    char dest[13];
    custom_memcpy(dest, src, sizeof(src));
    dest[12] = '\0';
    printf("Copied string: %s\n", dest);
    return 0;
}
