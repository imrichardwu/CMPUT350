#include <iostream>
#include <cassert>
#include <set>

int main() {
    std::set<int> values;
    int num;
    // Read integers from standard input
    while (std::cin >> num) {
        values.insert(num);
    }

    // Print the integers in sorted order
    for (int value : values) {
        std::cout << value << std::endl;
    }
}
