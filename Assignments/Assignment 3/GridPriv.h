// list private Grid members here

enum Tile { GROUND, WATER, BLOCKED };

// Grid dimensions
int width;
int height;

// Grid data
std::vector<std::vector<Tile>> tiles;

// Connected components cache
mutable std::vector<int> connectedComponents[3][3]; // [size][tileType]
mutable bool cacheValid[3][3];