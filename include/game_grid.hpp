#pragma once

#include "Board/board.h"

/// Tracks tower/solar cell placement on the 9x5 game board.
/// Inherits from academic Board to demonstrate OOP inheritance.
class PlacementGrid : public Board {
public:
    PlacementGrid(int rows, int cols);

    /// Returns true if cell (r,c) is occupied.
    bool isOccupied(int r, int c) const;

    /// Returns occupant char ('T', 'S', or ' ').
    char occupantType(int r, int c) const;

    /// Mark cell as occupied by type ('T' or 'S').
    void place(int r, int c, char type);

    /// Clear cell.
    void remove(int r, int c);

    static constexpr char kEmpty  = ' ';
    static constexpr char kTurret = 'T';
    static constexpr char kSolar  = 'S';
};
