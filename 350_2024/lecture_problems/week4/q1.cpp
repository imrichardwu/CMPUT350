#include <iostream>
using namespace std;

class X {
    int x;
};

class Y : public X {
    int y;
};

class Z : public X {
public:
    int z;
    virtual ~Z() = default;
};

// Part 1
/**
 * What is the output if you assume y is stored at address 1000
 * and we are using a 64-bit computer?
 */
void part1() {
    Y y{};
    cout << (void *)&y << endl;
    cout << (Y *)(&y) << endl;
    cout << reinterpret_cast<X *>(&y) << endl;
    cout << dynamic_cast<X *>(&y) << endl;
}

// Part 2
/**
 * What is the output if you assume z is stored at address 2000
 * and we are using a 64-bit computer?
 */
void part2() {
    Z z{};
    cout << (void *)&z << endl;                   // 0x7fff465195b0 ~ 2000
    cout << (X *)(&z) << endl;                    // 0x7fff465195b8 ~ 2008
    cout << reinterpret_cast<X *>(&z) << endl;    // 0x7fff465195b0 ~ 2000
    cout << dynamic_cast<X *>(&z) << endl;        // 0x7fff465195b8 ~ 2008
}
// Solution
/**
 * Casting a Z pointer to an X pointer skips Z's vftp
 * Y doesn't have a vftp, so all Y-related addresses are identical
 * reinterpret_cast doesn't care about actual types, it does what it says
 */

int main() {
    // part1();
    // part2();
    return 0;
}
