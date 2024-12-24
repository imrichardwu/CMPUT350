#include "Triang.h"

#include <cassert>

int main() {
    // Test function orientation_test
    Point a(0, 0), b(1, 1), c(2, 2); // Collinear points
    assert(orientation_test(a, b, c) == 0);
    std::cout << "orientation_test passed for collinear points" << std::endl;

    Point d(0, 0), e(1, 0), f(0, 1); // Counter-clockwise
    assert(orientation_test(d, e, f) > 0);
    std::cout << "orientation_test passed for counter-clockwise points" << std::endl;

    Point g(0, 0), h(0, 1), i(1, 0); // Clockwise
    assert(orientation_test(g, h, i) < 0);
    std::cout << "orientation_test passed for clockwise points" << std::endl;

    // Test case for circle_test
    Point j(0, 0), k(1, 0), l(0, 1);

    // Ensure points are in clockwise order for circle_test
    // Since orientation_test(j, k, l) > 0, they are counter-clockwise
    // Thus, we need to reverse the order to make them clockwise
    Coordinate orient_jkl = orientation_test(j, k, l);
    bool clockwise_jkl = (orient_jkl < 0);
    if (!clockwise_jkl) {
        std::swap(k, l);
    }

    // Debug output to understand the behavior
    Point m(2, 2); // Point expected to be outside
    auto result = circle_test(j, k, l, m);
    std::cout << "circle_test result for point outside: " << result << std::endl;
    assert(result > 0); // Outside the circle
    std::cout << "circle_test passed for point outside circle" << std::endl;

    // Test case for a point on the circle
    Point n(0, 0), o(1, 0), p(0, 1);
    // Ensure points are in clockwise order
    Coordinate orient_nop = orientation_test(n, o, p);
    bool clockwise_nop = (orient_nop < 0);
    if (!clockwise_nop) {
        std::swap(o, p);
    }
    Point q(1, 0); // On the circle
    result = circle_test(n, o, p, q);
    std::cout << "circle_test result for point on the circle: " << result << std::endl;
    assert(result == 0);
    std::cout << "circle_test passed for point on the circle" << std::endl;

    // Point inside the circle using an integer approximation
    Point r(0, 0), s(2, 0), t(0, 2);
    // Ensure points are in clockwise order
    Coordinate orient_rst = orientation_test(r, s, t);
    bool clockwise_rst = (orient_rst < 0);
    if (!clockwise_rst) {
        std::swap(s, t);
    }
    Point u(1, 1); // Inside the circle
    result = circle_test(r, s, t, u);
    std::cout << "circle_test result for point inside: " << result << std::endl;
    assert(result < 0); // Inside the circle
    std::cout << "circle_test passed for point inside circle" << std::endl;

    // Test case for rectangle_test
    Point rect_p1(0, 0), rect_p2(2, 2);
    Point inside_point(1, 1);
    Point border_point(2, 1);
    Point outside_point(3, 3);

    assert(rectangle_test(rect_p1, rect_p2, inside_point) < 0);
    std::cout << "rectangle_test passed for point inside rectangle" << std::endl;

    assert(rectangle_test(rect_p1, rect_p2, border_point) == 0);
    std::cout << "rectangle_test passed for point on rectangle border" << std::endl;

    assert(rectangle_test(rect_p1, rect_p2, outside_point) > 0);
    std::cout << "rectangle_test passed for point outside rectangle" << std::endl;

    return 0;
}