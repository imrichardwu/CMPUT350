#include <iostream>
using namespace std;

struct X {
    virtual ~X() = default;
};

struct Y : public X {};

struct Z {};

/**
 * Which type casts return a nullptr? Why?
 * Which can lead to undefined behaviour later?
 */
void test() {
    X x;
    Y y;
    X *px = dynamic_cast<X *>(&y);
    Y *py = dynamic_cast<Y *>(&x);
    Z *pz = dynamic_cast<Z *>(&x);

    X *px2 = reinterpret_cast<X *>(&y);
    Y *py2 = reinterpret_cast<Y *>(&x);
    Z *pz2 = reinterpret_cast<Z *>(&x);
}

int main() {
    cout << sizeof(X) << " " << sizeof(Y) << endl;

    X x;
    Y y;
    X *px = dynamic_cast<X *>(&y);
    Y *py = dynamic_cast<Y *>(&x);
    Z *pz = dynamic_cast<Z *>(&x);

    X *px2 = reinterpret_cast<X *>(&y);    // OK - by coincidence
    Y *py2 = reinterpret_cast<Y *>(&x);    // OUCH
    Z *pz2 = reinterpret_cast<Z *>(&x);    // OUCH

    cout << px << endl;     // 0x7fffe6bcd3c0
    cout << py << endl;     // 0    x is not a y
    cout << pz << endl;     // 0    z is not an x (Z has no relation to X)
    cout << px2 << endl;    // 0x7fffe6bcd3c0 (reinterpret_cast always succeeds)
    cout << py2 << endl;    // 0x7fffe6bcd3d0
    cout << pz2 << endl;    // 0x7fffe6bcd3d0

    return 0;
}
