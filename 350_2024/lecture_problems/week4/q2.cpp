#include <iostream>
using namespace std;

bool x = false;

int f() {
    x = !x;
    if (x) {
        return 2;
    } else {
        return 1;
    }
}

int g() {
    x = !x;
    if (x) {
        return 6;
    } else {
        return 3;
    }
}

// Question:
// What is the output of the following
int main() {
    cout << g() + f() << endl;
    // cout << f() + g() << endl;
    return 0;
}
