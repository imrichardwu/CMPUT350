#include <iostream>
#include <vector>

struct Lifetime {
    Lifetime() {
        std::cout << "Lifetime::Lifetime() constructor" << std::endl;
    }

    ~Lifetime() {
        std::cout << "Lifetime::~Lifetime() destructor" << std::endl;
    }
    Lifetime(const Lifetime &) = default;
    Lifetime(Lifetime &&) = default;
    Lifetime &operator=(const Lifetime &) = default;
    Lifetime &operator=(Lifetime &&) = default;
};

namespace {
void test1() {
    std::vector<Lifetime> vec;
    vec.reserve(2);
    for (int i = 0; i < 2; ++i) {
        vec.emplace_back();
    }
}

void test2() {
    std::vector<Lifetime *> vec;
    vec.reserve(2);
    for (int i = 0; i < 2; ++i) {
        vec.push_back(new Lifetime());    // NOLINT
    }
}
}    // namespace

int main() {
    std::cout << "TEST 1 BEGIN" << std::endl;
    test1();
    std::cout << "TEST 1 END" << std::endl;

    std::cout << "TEST 2 BEGIN" << std::endl;
    test2();
    std::cout << "TEST 2 END" << std::endl;
}
