// GridPriv.h

// Declare private helper functions
bool canOccupy(int size, int x, int y, Grid::Tile tile) const;
bool canMove(int size, int x1, int y1, int x2, int y2, Grid::Tile tile) const;
int heuristic(int x1, int y1, int x2, int y2) const;
void computeConnectedComponents(int size, Grid::Tile tileType) const;

// Declare private member variables
int width, height;
std::vector<Grid::Tile> tiles;
mutable std::map<std::pair<int, Grid::Tile>, std::vector<int>> connectedComponentsCache;
