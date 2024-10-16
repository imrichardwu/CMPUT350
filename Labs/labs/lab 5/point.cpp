#include <iostream>
#include <cassert>

template <typename T>
struct Point {
    T x, y; // Two coordinates of arbitrary type T

    // Constructor with default values as T{} 
    Point(T x0 = T{}, T y0 = T{}) : x(x0), y(y0) {}

    // Print member function
    void print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

    // Equality operator
    bool operator==(const Point& rhs) const {
        return (x == rhs.x && y == rhs.y);
    }
};

// Sum function template
template <typename T>
Point<T> sum(const Point<T> arr[], int n) {
    Point<T> result; // Default-initialized to (0, 0)
    for (int i = 0; i < n; ++i) {
        result.x += arr[i].x;
        result.y += arr[i].y;
    }
    return result;
}

// Find function template
template <typename T>
int find(const Point<T> arr[], int n, const Point<T>& p) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == p) {
            return i; // Return the index if point is found
        }
    }
    return -1; 
}


int main() {
    // Create an array of Point<int> objects
    Point<int> points[] = { {1, 2}, {3, 4}, {5, 6}, {1, 2} };
    int n = sizeof(points) / sizeof(points[0]);

    // Test sum function
    Point<int> total = sum(points, n);
    std::cout << "Sum of points: ";
    total.print(); 

    // Test find function
    Point<int> p(1, 2);
    int index = find(points, n, p);
    if (index != -1) {
        std::cout << "Point (1, 2) found at index: " << index << std::endl;
    } else {
        std::cout << "Point (1, 2) not found" << std::endl;
    }

    return 0;
}
