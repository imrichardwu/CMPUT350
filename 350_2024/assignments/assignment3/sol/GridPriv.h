// private Grid members

std::vector<Tile> data;
int width;
int height;

// can be changed by const functions
mutable std::vector<int> sectors[3];
mutable int nextSector[3];
mutable bool sectorsDirty;

void ResetSectors() const;
void TouchSector(int size, int x, int y) const;
void FloodFill(int size, int x, int y, int tile, int sector) const;
