#include <iostream>
#include <vector>

int main(int argc, char **argv) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int idx = 0;
    if (argc >= 2) {
        idx = std::atoi(argv[1]);
    }

    // std::cout << "Item at position " << idx << ": " << v[idx] << std::endl;
    std::cout << "Item at position " << idx << ": " << v.at(idx) << std::endl;
}
