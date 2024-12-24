#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "String.h"

using namespace std;

#define RUNTIME_TEST(condition) __RuntimeTest(condition, #condition)

static bool success(true);

static void __RuntimeTest(bool condition, const char *string) {
    cout << "\t" << string;
    for (size_t i(strlen(string)); i < 40; ++i) {
        cout << ' ';
    }
    cout << ": " << (condition ? "PASS" : "FAIL") << endl;
    success &= condition;
}

String Combine(String x, String y) {
    RUNTIME_TEST(strcmp(x.cstr(), "Hello") == 0);
    RUNTIME_TEST(strcmp(y.cstr(), "world!") == 0);

    // The ostringstream contains a temporary buffer. We will pass this data to
    // the constructor of String to test if String copies it to an internal
    // structure. When Combine() returns, the ostringstream will be destroyed
    // but the results should be preserved in the returned String.
    cout << "\nTesting constructor with temporary data..." << endl;
    ostringstream out;
    out << x.cstr() << " " << y.cstr();
    return String(out.str().c_str());
}

int main() {
    // Two SharedData structures should be allocated, and they should retain
    // copies of the string constants.

    cout << "Testing constructor with permanent data..." << endl;
    String a("Bonjour"), b("world!");
    RUNTIME_TEST(strcmp(a.cstr(), "Bonjour") == 0);
    RUNTIME_TEST(a.size() == 7);

    // Memory should be allocated to store "Hello". The memory that held
    // "Bonjour" should be freed during the assignment operator.

    cout << "\nTesting assignment operator..." << endl;
    a = String("Hello");
    RUNTIME_TEST(strcmp(a.cstr(), "Hello") == 0);
    RUNTIME_TEST(a.size() == 5);

    // We are passing strings by value, so the copy constructor will be used to
    // construct the arguments for Combine(). The return value will be assigned
    // to b, so the memory for "world!" should be freed after this operation,
    // but the memory for "Hello" should still be in use by a.

    cout << "\nTesting copy constructor..." << endl;

    b = Combine(a, b);

    RUNTIME_TEST(strcmp(b.cstr(), "Hello world!") == 0);
    RUNTIME_TEST(b.size() == 12);

    // Print out message based on whether tests succeeded or failed
    if (success) {
        cout << "\nAll tests passed, but remember to doublecheck your memory management." << endl;
        cout << "Your submission is required to free all memory allocated." << endl;
    } else {
        cout << "\nSome tests failed. Revisit your code and see if you made any mistakes." << endl;
    }

    return 0;
}
