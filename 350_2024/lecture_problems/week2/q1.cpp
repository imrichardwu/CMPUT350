// Consider the following struct
struct X {
    int a[200];    // NOLINT
    int b;
} mode;    // NOLINT

/**
 * a) How is moe laid out in memory, and how many bytes does it occupy?
 *    (assuming int values occupy 4 bytes)
 *
 * b) What is the best way of passing varaibles of type X as single parameter to:
 *    i)  a function that does change its parameter
 *    ii) a function that doesn't change its parameter?
 */

// Solution (a)
/**
 *    &moe  -> 0 ----------
 *              |    a    |    200 ints
 *          800 -----------
 *              |    b    |    int
 *          804 -----------
 *
 */

// Solution (b)
/**
 * Internally, references are represented as pointers,
 * which means instead of copying hundreds of bytes when passing
 * X by value, we only copy 8 bytes (single pointer on 64-bit architecture)
 * when passing by reference.
 *
 * Rule: Whenever you use a parameter more complex than an int, double, or
 * pointer, use a (const-) reference
 */
