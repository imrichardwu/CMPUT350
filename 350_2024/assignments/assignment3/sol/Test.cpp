#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>

#include "Grid.h"


using namespace std;

typedef std::unique_ptr<Grid> GridPtr;

GridPtr mymap;

void LoadMap(const char* filename);

int main() {
    LoadMap("map.bmp");

    if (!mymap.get()) {
        return -1;
    }

    std::cout << "Size\tStart\tGoal\tExpected\tReceived\tResult" << std::endl;

    while (true) {
        int size, x0, y0, x1, y1, connected, length(0);
        std::cin >> size >> x0 >> y0 >> x1 >> y1 >> connected;

        if (!std::cin) {
            break;
        }
        std::cout << (size + 1) << "x" << (size + 1) << "\t";
        std::cout << x0 << "," << y0 << "\t";
        std::cout << x1 << "," << y1 << "\t";

        if (connected) {
            std::cin >> length;
            std::cout << length << "\t\t";
        } else {
            std::cout << "no path\t\t";
        }

        const bool actualConnected(mymap->isConnected(size, x0, y0, x1, y1));
        bool pass(connected == actualConnected);

        if (actualConnected) {
            std::vector<Grid::Direction> path;
            const int actualLength(mymap->findShortestPath(size, x0, y0, x1, y1, path));
            pass = pass && (length == actualLength);
            std::cout << actualLength << "\t\t";
        } else {
            std::cout << "no path\t\t";
        }

        std::cout << (pass ? "PASS" : "FAIL") << std::endl;
    }

    return 0;
}

// Load an uncompressed 24-bit .bmp file into a grid
void LoadMap(const char* filename) {
    // Read the map file
    std::ifstream in(filename, std::ios::binary);

    if (!in) {
        std::cout << "Cannot read " << filename << std::endl;
        return;
    }

    // Read the map dimensions
    int width, height;
    assert(sizeof(width) == 4);
    in.seekg(18);
    in.read((char*)&width, 4);    // beware byte ordering!
    in.read((char*)&height, 4);
    mymap = GridPtr(new Grid(width, height));

    cout << "width=" << width << " height=" << height << endl;

    // Initialise the map
    in.seekg(54);

    for (int y(0); y < height; ++y) {
        for (int x(0); x < width; ++x) {
            // Load the image data
            unsigned char r, g, b;
            in.read((char*)&b, 1);
            in.read((char*)&g, 1);
            in.read((char*)&r, 1);

            // Store the tile data
            Grid::Tile tile(Grid::BLOCKED);
            if (g > 96) {
                tile = Grid::GROUND;
            }
            if (b > 96) {
                tile = Grid::WATER;
            }
            mymap->setTile(x, height - y - 1, tile);
        }
    }
}
