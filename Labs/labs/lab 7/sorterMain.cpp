#include "Sorter.h"

int main() {
    Sorter sorter;
    double ref_x, ref_y;
    
    // Read the first point (reference point)
    if (!(std::cin >> ref_x >> ref_y)) {
        std::cerr << "Error: Failed to read the reference point.\n";
        return 1;
    }

    // Read the remaining points and add them to the sorter
    double x, y;
    while (std::cin >> x >> y) {
        sorter.add_point(x, y);
    }

    // Print the sorted points based on their distance to the reference point
    sorter.print_sorted_closest_to(ref_x, ref_y);

    return 0;
}
