#include <iostream>
#include <list>

int main() {
    std::list<int> list1 = {1, 2, 3, 4};
    std::list<int> list2 = {5, 3, 4, 2};
    list1.merge(list2);
    for (auto x : list1)
        std::cout << x << '\n';
}
