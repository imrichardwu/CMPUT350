#include "Rational.h"

#include <cassert>
#include <iostream>

using namespace std;

Rational::Rational(int n, int d) {
    assert(d);
    if (d < 0) {
        n = -n;
        d = -d;
    }
    num = n;
    den = d;
}

//  should be moved to Rational.h to allow compiler to inline code
int Rational::get_num() const {
    return num;
}
int Rational::get_den() const {
    return den;
}

Rational Rational::operator+(const Rational &rhs) const {
    return {num * rhs.den + den * rhs.num, den * rhs.den};
}

Rational Rational::operator-(const Rational &rhs) const {
    return {num * rhs.den - den * rhs.num, den * rhs.den};
}

Rational Rational::operator*(const Rational &rhs) const {
    return {num * rhs.num, den * rhs.den};
}

Rational Rational::operator/(const Rational &rhs) const {
    assert(rhs.num);
    return {num * rhs.den, den * rhs.num};
}

bool Rational::operator==(const Rational &rhs) const {
    return num * rhs.den == den * rhs.num;
}

bool Rational::operator!=(const Rational &rhs) const {
    return !operator==(rhs);
}

//  prefix ++
Rational &Rational::operator++() {
    num += den;
    return *this;
}

//  postfix ++
Rational Rational::operator++(int) {
    Rational ret(*this);
    num += den;
    return ret;
}

ostream &operator<<(ostream &os, const Rational &rhs) {
    os << "[" << rhs.get_num() << "/" << rhs.get_den() << "]";
    return os;
}
