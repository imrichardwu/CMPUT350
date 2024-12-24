#include <iostream>
using namespace std;

static int y;
static int z;

int foo() {
    static int x;
    return ++x;
}

// Question
/**
 * In what order are x,y,z initialized and with what?
 * What is the output?
 */
int main() {
    cout << y << " " << z << endl;
    cout << foo() << endl;
    cout << foo() << endl;
    cout << foo() << endl;

    cout << foo() << foo() << foo() << endl;

    return 0;
}

// Solution
/*
  In what order are x,y,z initialized and with what?

    y,z,x

  - static variables in the same .cpp file are initialized
    in order => y,z = 0

    Across multiple .cpp files, static variable initialization
    order is unspecified

  - static function variables are initialized when the function is first run

    => x = 0 after y,z

  What is the output?

    0 0
    1
    2
    3
    654   (on the lab machines (g++ 5.4.0 (C++14)), 456 on g++ 10.2.1 (C++17))

  - static function variables retain their state across function calls

  - prior to C++17: the foo() call order in the last line is unspecified,
    although the actual output is done from left to right

  - C++17: evaluation order for << and >> is left-to-right. So, the first
    foo() is evaluated and printed first. However, in this example:

         f(g(), h(), j())

    the evaluation order of g(), h(), j() is unspecified, but their evaluations
    are not overlapping and f(...) is evaluated last

    Also, in

         getf()(g(), h(), j())

    (getf returning a function), getf() is evaluated prior to g(), h(), j()
*/
