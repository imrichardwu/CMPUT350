#include <iostream>

struct Point {
    int x{};
    int y{};
private:
    inline static int instance_count;

public:
    Point() {
        instance_count++;
    }

    ~Point() {
        instance_count--;
    }

    static int get_instance_count() {
        return instance_count;
    }
};


int main() {
    Point p1;
    std::cout << "Number of Point instances: " << Point::get_instance_count() << std::endl;
    Point p2;
    std::cout << "Number of Point instances: " << Point::get_instance_count() << std::endl;
    return 0;
}