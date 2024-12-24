
// Question
/**
 * Speculate as to why C++ distinguishes between new/delete
 *   from new[]/delete[]?
 *
 * Can't we just treat individual heap varaibles as arrays of length 1?
 */

// Solution
/**
 * C++ (mostly) follows the principle that you don't have to pay for
 * language features you are not using.
 *
 * How does delete[] p; know how long the array is that it is supposed
 * to free?
 *
 * The length needs to be stored somewhere. Where is implementation dependent.
 *
 * Often, the array length is stored right in front of the array,
 * and can be accessed by using negative indices ;-)
 * Try to see if this works for gcc/g++
 *
 * So in case of allocating arrays, we use one more (long) integer variable
 */
