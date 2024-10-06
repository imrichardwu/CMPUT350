// tests Set implementation

#include "Set.h"
#include <cassert>

using namespace std;

static const int INT_BITS = sizeof(int)*8;

Set::Set(size_t n) {
    this->n = n;
    this->a = (n + INT_BITS - 1) / INT_BITS; // Calculate the number of integers needed to store n bits
    this->bits = new unsigned int[a];
    for (size_t i = 0; i < a; i++) {
        bits[i] = 0;
    }
}

Set::~Set() {
    delete[] bits;
}

Set::Set(const Set &s) {
    n = s.n;
    a = s.a;
    bits = new unsigned int[a];
    for(size_t i = 0; i < a; i++){
        bits[i] = s.bits[i];
    }
}

Set &Set::operator=(const Set &s){
    if(this != &s) {
        unsigned int* newBits = new unsigned int[s.a];
        for(size_t i = 0; i < s.a; i++){
            newBits[i] = s.bits[i];
        }
        delete[] bits;
        bits = newBits;
        n = s.n;
        a = s.a;
    }
    return *this;
}


void Set::clear() {
    for(size_t i = 0; i < a; i++){
        bits[i] = 0;
    }
}

void Set::complement(){
    for(size_t i = 0; i < a; i++){
        bits[i] = ~bits[i]; // Invert all bits
    }

    size_t last = n % INT_BITS; // Find the number of bits in the last integer
    if(last != 0){
        bits[a - 1] &= (1U << static_cast<unsigned int>(last)) - 1; // Set the bits after n to 0
    }

}

 bool Set::has(size_t x) const {
    assert(x < n);
    size_t index = x / INT_BITS; // Find the index of the integer that contains the bit at x     
    size_t offset = x % INT_BITS; // Find the offset of the bit at x
    return (bits[index] & (1U << static_cast<unsigned int>(offset))) != 0; // Check if bit is set or not 
 }

void Set::add(size_t x){
    assert(x < n);
    size_t index = x / INT_BITS;
    size_t offset = x % INT_BITS;
    bits[index] |= (1U << static_cast<unsigned int>(offset)); // Set the bit at x to 1 
}

void Set::remove(size_t x){
    assert(x < n);
    size_t index = x / INT_BITS;
    size_t offset = x % INT_BITS;
    bits[index] &= ~(1U << static_cast<unsigned int>(offset)); // Set the bit at x to 0
}

void Set::add(const Set &s){
    assert(n == s.n);
    for(size_t i = 0; i < a; i++){
        bits[i]|= s.bits[i];
    }
}


void Set::remove(const Set &s) {
    assert(n == s.n);
    for(size_t i = 0; i < a; i++){
        bits[i] &= ~s.bits[i];
    }
}

void Set::print(std::ostream &os) const {
    os << "[ ";
    bool first = true;
    for(size_t i = 0; i < n; ++i) {
        if(has(i)){
            if(!first){
                os << " ";
            }
            os << i;
            first = false;
        }
    }
    if (!first) {
        os << " ";
    }
    os << "]";
}

int main() {
    Set a(64), b(64), c(64);
    Set d(65);

    a = b = c;

    a.add(1);
    a.add(5);
    b.add(3);
    b.remove(3);
    c.add(4);
    c.add(a);

    a.print(cout);
    cout << endl;
    b.print(cout);
    cout << endl;
    c.print(cout);
    cout << endl;

    a.complement();
    a.print(cout);
    cout << endl;
    cout << a.get_n() << endl;
    cout << a.has(1) << endl;
    cout << a.has(63) << endl;

    a.remove(a);
    a.print(cout);
    cout << endl;

    return 0;
}

/* output:

[ 1 5 ]
[ ]
[ 1 4 5 ]
[ 0 2 3 4 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43
44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 ] 64
0
1
[ ]

*/
