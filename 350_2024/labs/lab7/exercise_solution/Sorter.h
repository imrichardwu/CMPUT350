#ifndef SORTER_H_
#define SORTER_H_

#include <algorithm>
#include <iostream>
#include <vector>

class Sorter {
public:
    void add_point(double x, double y) {
        points.push_back(Point(x, y));
    }

    void print_sorted_closest_to(double x, double y) {
        std::sort(begin(points), end(points), ClosestTo(x, y));

        for (auto &p : points) {
            std::cout << p.x << " " << p.y << std::endl;
        }
    }

private:
    struct Point {
        double x, y;

        Point(double x = 0, double y = 0)
            : x(x), y(y) {}

        //  return distance squared between two points
        double dist2(const Point &other) const {
            const double dx(other.x - x), dy(other.y - y);
            return dx * dx + dy * dy;
        }
    };

    //  comparison functor
    //  contains point to compare with
    struct ClosestTo {
        ClosestTo(double x, double y)
            : point(x, y) {}

        bool operator()(const Point &a, const Point &b) const {
            return a.dist2(point) < b.dist2(point);
        }

        Point point;
    };

    //  stores all points
    std::vector<Point> points;
};

#endif    // SORTER_H_
