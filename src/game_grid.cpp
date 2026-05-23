#include "game_grid.hpp"

PlacementGrid::PlacementGrid(int rows, int cols)
    : Board(rows, cols)
{
    // All cells start as kEmpty (' ').
}

bool PlacementGrid::isOccupied(int r, int c) const {
    return GetCell(r, c) != kEmpty;
}

char PlacementGrid::occupantType(int r, int c) const {
    return GetCell(r, c);
}

void PlacementGrid::place(int r, int c, char type) {
    if (IsValid(r, c)) {
        SetCell(r, c, type);
    }
}

void PlacementGrid::remove(int r, int c) {
    if (IsValid(r, c)) {
        SetCell(r, c, kEmpty);
    }
}
