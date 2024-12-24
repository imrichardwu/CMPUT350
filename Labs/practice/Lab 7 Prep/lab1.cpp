#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

// Selection Sort Function Template
template <typename Iterator, typename Compare>
void selection_sort(Iterator first, Iterator last, Compare comp){
    for (Iterator i = first; i != last; ++i) {
        Iterator min = i;
        for (Iterator j = i; j != last; ++j) {
            if (comp(*j, *min)) {
                min = j;
            }
        }
        std::swap(*i, *min);
    }
};

template <typename T>
struct Myless {
    bool operator()(const T &a, const T &b) const {
        return a < b;
    }
};

template <typename T>
struct Mygreater {
    bool operator()(const T &a, const T &b) const {
        return a > b;
    }
};

int main() {
    std::vector<int> v{3, 5, 4};
    selection_sort(v.begin(), v.end(), Myless<int>());
    for (int i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    selection_sort(v.begin(), v.end(), Mygreater<int>());
    for (int i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;   
}