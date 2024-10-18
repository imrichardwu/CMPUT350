// Grid.cpp
#include "Grid.h"

// Intialize the grid with the given width and height
// Tile type of each cell is GROUND
Grid::Grid(int width, int height) : width(width), height(height), tiles(width * height, GROUND) {}

Grid::~Grid() {}

// Return the width of the grid
int Grid::getWidth() const {
    return width;
}

// Return the height of the grid
int Grid::getHeight() const {
    return height;
}

// Return the tile type at the given coordinates
Grid::Tile Grid::getTile(int x, int y) const {
    // y * width + x is the index of the cell at (x, y)
    return tiles[y * width + x];
}

// Set the tile type at the given coordinates
void Grid::setTile(int x, int y, Tile tile) {
    // y * width + x is the index of the cell at (x, y)
    tiles[y * width + x] = tile;  // Set the tile type at the given coordinates to the given tile type 
    // Clear cache when map changes
    connectedComponentsCache.clear(); 
}

// Check if an object with the given size can occupy the given cell
bool Grid::canOccupy(int size, int x, int y, Tile tile) const {
    for (int i = 0; i <= size; i++) { // Loop through the x-axis
        for (int j = 0; j <= size; j++) { // Loop through the y-axis
            // Check if the cell is out of bounds or if the cell is not of the given tile type
            if (x + i >= width || y + j >= height || getTile(x + i, y + j) != tile) {
                return false;
            }
        }
    }
    return true; // Return true if the object can occupy the given cell
}

// Check if an object with the given size can move from (x1, y1) to (x2, y2)
bool Grid::canMove(int size, int x1, int y1, int x2, int y2, Tile tile) const {
    // Get the minimum and maximum x and y coordinates of the object to be moved
    int minX = std::min(x1, x2);
    int minY = std::min(y1, y2);
    int maxX = std::max(x1, x2);
    int maxY = std::max(y1, y2);

    for (int y = minY; y <= maxY + size; y++) { // Loop through the y-axis
        for (int x = minX; x <= maxX + size; x++) { // Loop through the x-axis
            if (x >= width || y >= height || getTile(x, y) != tile) { // Check if the cell is out of bounds or if the cell is not of the given tile type
                return false;
            }
        }
    }
    return true; // Return true if the object can move from (x1, y1) to (x2, y2)
}

// Check if the given cells are connected
bool Grid::isConnected(int size, int x1, int y1, int x2, int y2) const {
    Tile tileType = getTile(x1, y1); // Get the tile type at the given coordinates
    // Check if the tile type at the given coordinates is not the same as the tile type at the other coordinates
    if (tileType != getTile(x2, y2) || !canOccupy(size, x1, y1, tileType) || !canOccupy(size, x2, y2, tileType)) {
        return false; // Return false if the given cells are not connected
    }

    // Check if the connected components cache does not contain the given size and tile type
    auto cacheKey = std::make_pair(size, tileType);
    if (connectedComponentsCache.find(cacheKey) == connectedComponentsCache.end()) { // If the cache does not contain the given size and tile type
        computeConnectedComponents(size, tileType);
    }

    // Get the connected components of the given size and tile type
    const auto& components = connectedComponentsCache[cacheKey];
    return components[y1 * width + x1] == components[y2 * width + x2]; // Cells are connceted if they are in the same connected component
}

// Calculate the heuristic cost between two cells
int Grid::heuristic(int x1, int y1, int x2, int y2) const {
    int dx = std::abs(x2 - x1); // Absolute difference between the x-coordinates
    int dy = std::abs(y2 - y1); // Absolute difference between the y-coordinates
    // Return the heuristic cost between the two cells
    return CARDINAL_COST * (dx + dy) + (DIAGONAL_COST - 2 * CARDINAL_COST) * std::min(dx, dy);
}

// Compute the connected components of the given size and tile type
void Grid::computeConnectedComponents(int size, Grid::Tile tileType) const {
    std::vector<int> components(width * height, -1); // Initialize the components vector with -1
    int componentId = 0;

    for (int y = 0; y < height - size; y++) { // Loop through Y
        for (int x = 0; x < width - size; x++) { // Loop through X
            // Check if the cell is not part of a connected component and if the object can occupy the cell
            if (components[y * width + x] == -1 && canOccupy(size, x, y, tileType)) {
                // Perform a flood fill to find the connected components
                std::queue<std::pair<int, int>> queue;
                queue.push({x, y});
                components[y * width + x] = componentId; // Set the component id of the cell

                while (!queue.empty()) {
                    auto [cx, cy] = queue.front();
                    queue.pop();

                    for (int dy = -1; dy <= 1; dy++) { // Loop through Y
                        for (int dx = -1; dx <= 1; dx++) { // Loop through X
                            int nx = cx + dx, ny = cy + dy; // Get new coordinates
                            // Check if the new coordinates are within bounds and if the cell is not part of a connected component
                            if (nx >= 0 && nx < width - size && ny >= 0 && ny < height - size &&
                                components[ny * width + nx] == -1 && canOccupy(size, nx, ny, tileType) &&
                                canMove(size, cx, cy, nx, ny, tileType)) {
                                components[ny * width + nx] = componentId;
                                queue.push({nx, ny});
                            }
                        }
                    }
                }
                componentId++;
            }
        }
    }
    connectedComponentsCache[std::make_pair(size, tileType)] = components;
}

int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, std::vector<Direction> &path) const {
    path.clear();
    // Check given cells are not connected
    if (!isConnected(size, x1, y1, x2, y2)) {
        return -1;
    }

    // A* algorithm
    struct Node {
        int x, y, g, f; // x and y coordinates, cost from start to current node, total cost
        Direction dir; // direction to reach this node
        Node* parent; // parent node
        
        Node(int x, int y, int g, int f, Direction dir, Node* parent)
            : x(x), y(y), g(g), f(f), dir(dir), parent(parent) {}
    };
    
    // Compare function for priority queue
    auto compare = [](const Node* a, const Node* b) { return a->f > b->f; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openList(compare); 
    std::vector<std::vector<bool>> closedList(height, std::vector<bool>(width, false)); 

    Node* start = new Node(x1, y1, 0, heuristic(x1, y1, x2, y2), N, nullptr); // Start node
    openList.push(start);

    
    while (!openList.empty()) {
        Node* current = openList.top(); // Get the node with the lowest cost
        openList.pop();

        // Check if the current node is the destination
        if (current->x == x2 && current->y == y2) {
            int cost = current->g; // Get the cost of the path
            
            // Reconstruct path
            while (current->parent != nullptr) {
                path.push_back(current->dir); 
                current = current->parent;
            }
            std::reverse(path.begin(), path.end()); // Reverse the path to get the correct order
            return cost;
        }

        // Avoid revisiting nodes
        if (closedList[current->y][current->x]) {
            continue;
        }
        closedList[current->y][current->x] = true; // Mark node as visited

        static const int dx[] = {0, 1, 1, 1, 0, -1, -1, -1}; 
        static const int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1}; 
        static const Direction dirs[] = {N, NE, E, SE, S, SW, W, NW};
        static const int costs[] = {CARDINAL_COST, DIAGONAL_COST, CARDINAL_COST, DIAGONAL_COST,
                                    CARDINAL_COST, DIAGONAL_COST, CARDINAL_COST, DIAGONAL_COST};

        for (int i = 0; i < 8; i++) { // Go through all directions
            int nx = current->x + dx[i], ny = current->y + dy[i];
            // Check if the new coordinates are within bounds and if the cell is not visited
            if (nx >= 0 && nx < width - size && ny >= 0 && ny < height - size &&
                !closedList[ny][nx] && canMove(size, current->x, current->y, nx, ny, getTile(current->x, current->y))) {
                int ng = current->g + costs[i]; // New cost from start to current node
                int nf = ng + heuristic(nx, ny, x2, y2); // New total cost
                Node* neighbor = new Node(nx, ny, ng, nf, dirs[i], current); 
                openList.push(neighbor);
            }
        }
    }

    return -1;
}
