#include "Grid.h"

Grid::Grid(int width, int height) : width(width), height(height) {
    tiles = std::vector<std::vector<Tile>>(height, std::vector<Tile>(width, Tile::GROUND));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cacheValid[i][j] = false;
        }
    }
}

Grid::~Grid() {}

int Grid::getWidth() const {
    return width;
}

int Grid::getHeight() const {
    return height;
}

Grid::Tile Grid::getTile(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return Tile::BLOCKED;
    }
    return tiles[y][x];
}

void Grid::setTile(int x, int y, Tile tile) {
    assert(x >= 0 && x < width && y >= 0 && y < height);  // Check valid indices
    tiles[y][x] = tile;

    // Invalidate cache only for the relevant tile type.
    for (int i = 0; i < 3; i++) {
        cacheValid[i][static_cast<int>(tile)] = false;
    }
}

bool Grid::canOccupy(int size, int x, int y, Tile tileType) const {
    for (int i = y; i < y + size; i++) {
        for (int j = x; j < x + size; j++) {
            if (getTile(j, i) != tileType) {
                return false;
            }
        }
    }
    return true;
}

bool Grid::canMove(int size, int x1, int y1, int x2, int y2, Tile tileType) const {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int absDx = abs(dx);
    int absDy = abs(dy);
    int stepX = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
    int stepY = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);

    if (absDx == 0 && absDy == 0) {
        return true;
    }

    if (absDx > 0 && absDy > 0) {
        return false;
    }

    if (absDx > 0) {
        for (int i = 0; i < absDx; i++) {
            if (!canOccupy(x1 + i * stepX, y1, size, tileType)) {
                return false;
            }
        }
    } else {
        for (int i = 0; i < absDy; i++) {
            if (!canOccupy(x1, y1 + i * stepY, size, tileType)) {
                return false;
            }
        }
    }

    return true;
}

void Grid::computeConnectedComponents(int size, Tile tileType) const {
    if (cacheValid[size][static_cast<int>(tileType)]) {
        return;
    }

    connectedComponents[size][static_cast<int>(tileType)] = std::vector<int>(width * height, -1);
    std::vector<std::pair<int, int>> queue;
    int component = 0;

    const int dx[4] = { 0, 1, 0, -1 };
    const int dy[4] = { -1, 0, 1, 0 };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (getTile(x, y) == tileType && connectedComponents[size][static_cast<int>(tileType)][y * width + x] == -1) {
                queue.push_back(std::make_pair(x, y));
                connectedComponents[size][static_cast<int>(tileType)][y * width + x] = component;

                while (!queue.empty()) {
                    int curX = queue.back().first;
                    int curY = queue.back().second;
                    queue.pop_back();

                    for (int i = 0; i < 4; i++) {
                        int newX = curX + dx[i];
                        int newY = curY + dy[i];

                        if (newX >= 0 && newX < width && newY >= 0 && newY < height && getTile(newX, newY) == tileType && connectedComponents[size][static_cast<int>(tileType)][newY * width + newX] == -1) {
                            queue.push_back(std::make_pair(newX, newY));
                            connectedComponents[size][static_cast<int>(tileType)][newY * width + newX] = component;
                        }
                    }
                }

                component++;
            }
        }
    }

    cacheValid[size][static_cast<int>(tileType)] = true;
}

bool Grid::isConnected(int size, int x1, int y1, int x2, int y2) const {
    Tile tileType = getTile(x1, y1);
    if (tileType != getTile(x2, y2)) {
        return false;
    }
    if (!canOccupy(x1, y1, size, tileType) || !canOccupy(x2, y2, size, tileType)) {
        return false;
    }

    computeConnectedComponents(size, tileType);
    return connectedComponents[size][static_cast<int>(tileType)][y1 * width + x1] == connectedComponents[size][static_cast<int>(tileType)][y2 * width + x2];
}

// Heuristic function for A*
int Grid::heuristic(int x1, int y1, int x2, int y2) const {
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return CARDINAL_COST * (dx + dy) + (DIAGONAL_COST - 2 * CARDINAL_COST) * std::min(dx, dy);
}

int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, std::vector<Direction> &path) const {
    path.clear();

    if (x1 < 0 || x1 >= width || y1 < 0 || y1 >= height ||
        x2 < 0 || x2 >= width || y2 < 0 || y2 >= height) {
        return -1;
    }
    Tile tileType = getTile(x1, y1);
    if (tileType == Tile::BLOCKED) {
        return -1;
    }
    if (getTile(x2, y2) != tileType) {
        return -1;
    }
    if (!canOccupy(x1, y1, size, tileType)) {
        return -1;
    }
    if (!canOccupy(x2, y2, size, tileType)) {
        return -1;
    }

    struct Node {
        int x, y;
        int g; // Cost from start to current node
        int h; // Heuristic estimate to goal
        int f; // Total estimated cost
        int parentX, parentY;
    };

    struct CompareNode {
        bool operator()(const Node &a, const Node &b) const {
            if (a.f == b.f)
                return a.g > b.g; // Tie breaker
            return a.f > b.f;
        }
    };

    std::priority_queue<Node, std::vector<Node>, CompareNode> openList;
    std::set<std::pair<int, int>> closedSet;
    std::map<std::pair<int, int>, Node> allNodes;

    Node startNode = { x1, y1, 0, 0, 0, -1, -1 };
    startNode.h = heuristic(x1, y1, x2, y2);
    startNode.f = startNode.g + startNode.h;

    openList.push(startNode);
    allNodes[{x1, y1}] = startNode;

    const int dxs[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    const int dys[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    const Direction dirs[8] = { N, NE, E, SE, S, SW, W, NW };
    const int moveCost[8] = { CARDINAL_COST, DIAGONAL_COST, CARDINAL_COST, DIAGONAL_COST,
                              CARDINAL_COST, DIAGONAL_COST, CARDINAL_COST, DIAGONAL_COST };

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        if (current.x == x2 && current.y == y2) {
            // Reconstruct path
            std::vector<Direction> reversePath;
            int cost = current.g;
            Node *node = &current;
            while (!(node->x == x1 && node->y == y1)) {
                int px = node->parentX;
                int py = node->parentY;
                int dirIndex = -1;
                int deltaX = node->x - px;
                int deltaY = node->y - py;
                for (int i = 0; i < 8; ++i) {
                    if (deltaX == dxs[i] && deltaY == dys[i]) {
                        dirIndex = i;
                        break;
                    }
                }
                if (dirIndex != -1) {
                    reversePath.push_back(dirs[dirIndex]);
                } else {
                    // Should not happen
                    return -1;
                }
                node = &allNodes[{px, py}];
            }
            path.assign(reversePath.rbegin(), reversePath.rend());
            return cost;
        }

        if (closedSet.count({ current.x, current.y }) > 0)
            continue;

        closedSet.insert({ current.x, current.y });

        for (int i = 0; i < 8; ++i) {
            int nx = current.x + dxs[i];
            int ny = current.y + dys[i];
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                if (closedSet.count({ nx, ny }) > 0)
                    continue;
                if (!canOccupy(nx, ny, size, tileType))
                    continue;
                if (!canMove(size, current.x, current.y, nx, ny, tileType))
                    continue;
                int tentative_g = current.g + moveCost[i];
                auto it = allNodes.find({ nx, ny });
                if (it == allNodes.end() || tentative_g < it->second.g) {
                    Node neighbor;
                    neighbor.x = nx;
                    neighbor.y = ny;
                    neighbor.g = tentative_g;
                    neighbor.h = heuristic(nx, ny, x2, y2);
                    neighbor.f = neighbor.g + neighbor.h;
                    neighbor.parentX = current.x;
                    neighbor.parentY = current.y;
                    openList.push(neighbor);
                    allNodes[{ nx, ny }] = neighbor;
                }
            }
        }
    }
    return -1;
}
