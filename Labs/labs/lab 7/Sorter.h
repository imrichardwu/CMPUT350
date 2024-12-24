#ifndef SORTER_H_
#define SORTER_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class Sorter {
public:
    void add_point(double x, double y) {
        points.push_back(std::make_pair(x, y)); // Add the point to the vector
    }

    void print_sorted_closest_to(double x, double y) {
        // Sort the vector by distance from the given point
        std::sort(points.begin(), points.end(), [x, y](const std::pair<double, double> &p1, const std::pair<double, double> &p2) {
            return std::sqrt(std::pow(p1.first - x, 2) + std::pow(p1.second - y, 2)) < std::sqrt(std::pow(p2.first - x, 2) + std::pow(p2.second - y, 2));
        });

        // Print the sorted points
        for(const auto &p : points){
            std::cout << p.first << " " << p.second << "\n";
        }
    }

private:
    // data members and helper functions go here
    std::vector<std::pair<double, double>> points; // Vector to store the point
};

#endif    // SORTER_H_
