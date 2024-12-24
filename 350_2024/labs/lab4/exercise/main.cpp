
int main() {
    Rational u;            // value 0
    Rational v(2);         // value 2
    Rational w(-3, -4);    // value 3/4

    Rational x(v);
    Rational y;
    y = w;

    Rational a = x;
    Rational b = y;
    Rational c = ++ ++a;
    Rational d = b++;

    std::cout << "u=" << u << std::endl;
    std::cout << "v=" << v << std::endl;
    std::cout << "w=" << w << std::endl;
    std::cout << "x=" << x << std::endl;
    std::cout << "y=" << y << std::endl;

    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
    std::cout << "c=" << c << std::endl;
    std::cout << "d=" << d << std::endl;

    std::cout << "result1=" << u + v - w / x * y << " result2=" << w + x << std::endl;

    if (x == x && x != y) {
        std::cout << "==,!= work" << std::endl;

    } else {
        std::cout << "==,!= don't work" << std::endl;
    }

    if (x == v) {
        std::cout << "CC works" << std::endl;

    } else {
        std::cout << "CC doesn't work" << std::endl;
    }

    if (y != w) {
        std::cout << "AO doesn't work" << std::endl;
    } else {
        std::cout << "AO works" << std::endl;
    }

    // More tests ...

    return 0;
}

/*
  output:

  u=[0/1]
  v=[2/1]
  w=[3/4]
  x=[2/1]
  y=[3/4]
  a=[4/1]
  b=[7/4]
  c=[4/1]
  d=[3/4]
  result1=[55/32] result2=[11/4]
  ==,!= work
  CC works
  AO works

*/
