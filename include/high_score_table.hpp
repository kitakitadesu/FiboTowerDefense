#pragma once

#include <string>
#include <vector>

#include "Board/board.h"

struct HighScoreEntry {
    std::string name;
    int score;
};

/// Stores top high-scores in a fixed-size grid (kMaxEntries × 2).
/// Inherits from academic Board to demonstrate a second OOP inheritance point.
class HighScoreTable : public Board {
public:
    HighScoreTable();

    /// Insert a new score; only top kMaxEntries are kept.
    void addEntry(const std::string& name, int score);

    /// Return top N entries sorted highest-first.
    std::vector<HighScoreEntry> getTopEntries(int count) const;

    static constexpr int kMaxEntries = 10;
};
