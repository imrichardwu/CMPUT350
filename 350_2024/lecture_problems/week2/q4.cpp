// Question
/**
 * Write a function that takes an integer and a reference to an output stream
 * (type std::ostream) as parameters, and prints the decimal value
 * of the int to that stream.
 *
 * By default, the output stream reference must refer to std::cout
 */

// Solution
#include <iostream>
using namespace std;    // declares global stream variables cin,cout,cerr

// the default value of os is a reference to std::cout
namespace {
void print(int value, ostream &os = std::cout) {
    os << value;
}
}    // namespace

int main() {
    print(5);               // to cout
    print(6, std::cout);    // to cout
    print(7, std::cerr);    // to cerr

    return 0;
}

/*
  What is the effect of using cout and cerr?

  ./a.out               prints 567 to terminal window
  ./a.out > aaa         prints 7 and stores 56 in file aaa
  ./a.out > aaa 2> bbb  doesn't print anything, but stores 56 in file aaa and 7 in bbb
  ./a.out > aaa 2>&1    doesn't print anything, but stores 567 in file aaa

  > redirects the stdout output to a file  (same effect as 1>, stdout has file descriptor 1)
  2> redirects the stderr output to a file (stderr has fd 2)
  2>&1 redirects stderr output to stdout before saving

  ( ./a.out &> aaa    : has the same effect as previous line,
    ./a.out &>> aaa   : works too (append both) )

  1 is the file descriptor of stdout
  2 is the file descriptor of stderr
  0 is the file descriptor of stdin

  For more on file redirection consult the bash documentation

  Good practice:  use stdout for output you may want to store or pipe into other commands
                  use stderr for diagnostic messages and error reports

  In C:   printf(...)                (stdout)
          fprintf(stdout, ...)
          fprintf(stderr, ...)

  In C++: cout << ...
          cerr << ...

  In C++ it is safe to mix printf and ostream outputs - they will appear in order
*/
