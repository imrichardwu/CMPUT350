#ifndef RATIONAL_H_
#define RATIONAL_H_

#include <iosfwd>

class Rational {
public:
    Rational(int n = 0, int d = 1);

    //  class operators
    Rational operator+(const Rational &rhs) const;
    Rational operator-(const Rational &rhs) const;
    Rational operator*(const Rational &rhs) const;
    Rational operator/(const Rational &rhs) const;

    bool operator==(const Rational &rhs) const;
    bool operator!=(const Rational &rhs) const;

    //  pre-increment, adds 1, returns reference to object
    Rational &operator++();

    //  post-increment, adds 1, returns previous value
    Rational operator++(int);

    int get_num() const;
    int get_den() const;

private:
    //  only data
    int num, den;
};

std::ostream &operator<<(std::ostream &os, const Rational &rhs);

#endif    // RATIONAL_H_
