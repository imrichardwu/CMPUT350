#include <iostream>

template<typename T, size_t N>
class Array {
public:
    Array() {
        for (size_t i = 0; i < N; i++) {
            data[i] = T();
        }
    }

    size_t size() const {
        return N;
    }

    T &operator[](size_t i) {
        return data[i];
    }

    const T &operator[](size_t i) const {
        return data[i];
    }
    
private:
    T data[N];
};

int main() {
    Array<int, 3> A1;
    Array<double, 5> A2;
    std::cout << A1.size() << std::endl;
    std::cout << A2.size() << std::endl;
    A1[0] = 2;
    std::cout << A1[0] << std::endl;
    return 0;
}