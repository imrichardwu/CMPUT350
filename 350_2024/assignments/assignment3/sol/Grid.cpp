#include "Grid.h"

#include <cassert>
#include <cstdlib>
#include <queue>


// helper struct to represent coordinates

struct Point {
    int x, y;

    Point() : x(-1), y(-1) {}
    Point(int x, int y) : x(x), y(y) {}

    Point move(Grid::Direction direction) const {
        static const int offsets[][2] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
        return Point(x + offsets[direction][0], y + offsets[direction][1]);
    }

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }
};

// retrieve the cost of moving in a particular direction
static int getMoveCost(Grid::Direction direction) {
    static const int costs[8] = {Grid::CARDINAL_COST, Grid::DIAGONAL_COST, Grid::CARDINAL_COST, Grid::DIAGONAL_COST,
                                 Grid::CARDINAL_COST, Grid::DIAGONAL_COST, Grid::CARDINAL_COST, Grid::DIAGONAL_COST};
    return costs[direction];
}

// estimate the cost of moving between two points
static int estimateCost(const Point &a, const Point &b) {
    const int dx(abs(b.x - a.x));
    const int dy(abs(b.y - a.y));
    return dx > dy ? dy * Grid::DIAGONAL_COST + (dx - dy) * Grid::CARDINAL_COST
                   : dx * Grid::DIAGONAL_COST + (dy - dx) * Grid::CARDINAL_COST;
}

// create a grid with set width and height, initialise all tiles to GROUND
Grid::Grid(int width, int height) : data(width * height, GROUND), width(width), height(height) {
    assert(width > 0 && height > 0);

    for (size_t i = 0; i < 3; ++i) {
        sectors[i].resize(width * height, 0);
        nextSector[i] = 1;
    }
    sectorsDirty = false;
}

Grid::~Grid() {}

int Grid::getWidth() const {
    return width;
}

int Grid::getHeight() const {
    return height;
}

// return the tile at (x, y), where 0 <= x < width and 0 <= y < height
Grid::Tile Grid::getTile(int x, int y) const {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    return data[y * width + x];
}

// set the tile at (x, y), where 0 <= x < width and 0 <= y < height,
// and tile is a valid Tile type.
void Grid::setTile(int x, int y, Tile tile) {
    assert(tile >= GROUND && tile <= BLOCKED);
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    data[y * width + x] = tile;
    sectorsDirty = true;
}

void Grid::ResetSectors() const {
    for (size_t i = 0; i < 3; ++i) {
        sectors[i].clear();
        sectors[i].resize(width * height, 0);
        nextSector[i] = 1;
    }
    sectorsDirty = false;
}

void Grid::TouchSector(int size, int x, int y) const {
    // if the map has been written to, throw away the sectors
    if (sectorsDirty) {
        ResetSectors();
    }

    // if this point has its sector assigned, return early
    if (sectors[size][y * width + x]) {
        return;
    }

    // otherwise assign a sector by floodfilling from this point
    FloodFill(size, x, y, getTile(x, y), nextSector[size]);
    ++nextSector[size];
}

void Grid::FloodFill(int size, int x, int y, int tile, int sector) const {
    // skip if we have wandered off the map boundaries
    if (x < 0 || y < 0 || x + size >= width || y + size >= height) {
        return;
    }

    // skip this spot if a sector was assigned here
    int &spotSector(sectors[size][y * width + x]);

    if (spotSector != 0) {
        return;
    }

    // skip this spot if the terrain is wrong
    for (int i = 0; i <= size; ++i) {
        for (int j = 0; j <= size; ++j) {
            if (getTile(x + j, y + i) != tile) {
                return;
            }
        }
    }

    // assign this sector
    spotSector = sector;

    // flood out in cardinal directions only for accessibility
    FloodFill(size, x - 1, y, tile, sector);
    FloodFill(size, x + 1, y, tile, sector);
    FloodFill(size, x, y - 1, tile, sector);
    FloodFill(size, x, y + 1, tile, sector);
}

// return true iff object with a given size can reside on (x1, y1)
// and move from there to (x2, y2) while staying at the same tile
// type without colliding.
//
// this should execute FASTER than calling findShortestPath().
bool Grid::isConnected(int size, int x1, int y1, int x2, int y2) const {
    // start location is on the map
    assert(x1 >= 0 && x1 + size < width);
    assert(y1 >= 0 && y1 + size < height);

    // end location is on the map
    assert(x2 >= 0 && x2 + size < width);
    assert(y2 >= 0 && y2 + size < height);

    // points are connected if they are accessible
    // and belong to the same sector.
    TouchSector(size, x1, y1);
    const int sector1(sectors[size][y1 * width + x1]);
    const int sector2(sectors[size][y2 * width + x2]);
    return sector1 == sector2 && sector1 != 0;
}

// data structures used for searching
struct Node {
    using Action = Grid::Direction;

    Point state;      // Current state
    Node *parent;     // Parent node
    Action action;    // Action taken to reach state
    int gCost;        // Cost to reach state
    int fCost;        // Total heuristic cost of state

    Node(const Point &start, const Point &goal) : state(start), parent(0), gCost(0), fCost(estimateCost(start, goal)) {}

    Node(Node *parent, Action action, const Point &goal)
        : state(parent->state.move(action)),
          parent(parent),
          action(action),
          gCost(parent->gCost + getMoveCost(action)),
          fCost(gCost + estimateCost(state, goal)) {}
};

struct LeastCost {
    bool operator()(const Node *a, const Node *b) const {
        if (a->fCost > b->fCost) {
            return true;
        }
        if (a->fCost < b->fCost) {
            return false;
        }
        return a->gCost < b->gCost;
    }
};

using OpenList = std::priority_queue<Node *, std::vector<Node *>, LeastCost>;
using ClosedList = std::vector<Node *>;

// compute a shortest path from (x1, y1) to (x2, y2) for an object with a
// given size. Return the cost of the shortest path or -1 if there is no path
int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, std::vector<Direction> &path) const {
    // start location is on the map
    assert(x1 >= 0 && x1 + size < width);
    assert(y1 >= 0 && y1 + size < height);

    // start location is of a consistent tile type
    const Tile tile(getTile(x1, y1));

    for (int y(y1); y <= y1 + size; ++y) {
        for (int x(x1); x <= x1 + size; ++x) {
            assert(getTile(x, y) == tile);
        }
    }

    // end location is on the map
    const Point goal(x2, y2);
    assert(x2 >= 0 && x2 + size < width);
    assert(y2 >= 0 && y2 + size < height);

    // initialise search
    int cost = -1;
    OpenList open;
    ClosedList closed(width * height, nullptr);

    // heap allocation OK for this assignment, but too slow for high-performance
    // pathfinding
    open.push(new Node(Point(x1, y1), goal));

    // while we have more nodes on the open list
    while (!open.empty()) {
        // get information about the current node
        Node *curNode = open.top();
        open.pop();
        const Point &curState(curNode->state);
        const int curIndex(curState.y * width + curState.x);

        // if state was previously closed

        if (closed[curIndex]) {
            // free node memory and skip
            delete curNode;
            continue;
        }

        // close the state corresponding to this node
        closed[curIndex] = curNode;

        // if we have reached the goal state
        if (curState == goal) {
            // store the final cost
            cost = curNode->gCost;

            // retrieve the directions for the path
            std::vector<Direction> revPath;

            while (curNode->parent) {
                revPath.push_back(curNode->action);
                curNode = curNode->parent;
            }
            path.assign(revPath.rbegin(), revPath.rend());

            // stop searching
            break;
        }

        // enqueue neighboring states
        for (int i = 0; i < 8; ++i) {
            // determine the next state
            const Direction direction(static_cast<Direction>(i));
            const Point nextState(curState.move(direction));

            // skip moves that move us off the edge of the map
            const int moveX0(std::min(curState.x, nextState.x));
            const int moveY0(std::min(curState.y, nextState.y));
            const int moveX1(std::max(curState.x, nextState.x) + size);
            const int moveY1(std::max(curState.y, nextState.y) + size);

            if (moveX0 < 0 || moveX1 >= width || moveY0 < 0 || moveY1 >= height) {
                continue;
            }

            // skip moves that move us to previously closed states
            // [ assumes consistent heuristic, which is true ]
            if (closed[nextState.y * width + nextState.x]) {
                continue;
            }

            // skip moves that are obstructed by terrain
            if (!isConnected(size, curState.x, curState.y, nextState.x, nextState.y) ||
                !isConnected(size, curState.x, curState.y, nextState.x, curState.y) ||
                !isConnected(size, curState.x, curState.y, curState.x, nextState.y)) {
                continue;
            }

            // enqueue next node
            open.push(new Node(curNode, direction, goal));
        }
    }

    // free remaining nodes on open list
    while (!open.empty()) {
        delete open.top();
        open.pop();
    }

    // free nodes on closed list
    for (auto it : closed) {
        delete it;
    }

    // return the final cost of the path (or -1 if we didn't find the goal)
    return cost;
}
