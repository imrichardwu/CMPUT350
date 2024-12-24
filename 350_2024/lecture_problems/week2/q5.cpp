// Question
/**
 * What are the values of x, y, z after this code snippet runs?
 */
void foo() {
    int x = 6;
    int &y = x;
    int &z = y;
    ++x;
    ++y;
    ++z;
}

// Solution
#include <iostream>
using namespace std;

int main() {
    int x = 6;
    int &y = x;    // y is an alias for x
    int &z = y;    // z is an alias for y, so z is an alias of x as well
    // thus, all operations below act on variable x
    ++x;
    ++y;
    ++z;
    cout << x << " " << y << " " << z << endl;    // 9 9 9
    return 0;
}
