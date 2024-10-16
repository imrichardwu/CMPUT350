#include <iostream>

template<typename T>
void swap_if_bigger(T &a, T &b) {
    if (a > b) {
        T temp = a;
        a = b;
        b = temp;
    }
}

int main() {
    int a = 2, b = 1;
    swap_if_bigger(a, b);
    std::cout << a << " " << b << std::endl;

    double c = 0.5, d = 1.3;
    swap_if_bigger(c, d);
    std::cout << c << " " << d << std::endl;
    return 0;
}