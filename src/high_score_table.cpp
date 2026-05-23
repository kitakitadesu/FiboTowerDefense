#include "high_score_table.hpp"

#include <algorithm>
#include <cstring>
#include <sstream>

HighScoreTable::HighScoreTable()
    : Board(kMaxEntries, 2)   // 10 rows × 2 cols (name, score)
{
}

void HighScoreTable::addEntry(const std::string& name, int score) {
    // Shift down and find insertion spot
    for (int r = 0; r < num_rows_; ++r) {
        // Parse existing score from col 1
        int existingScore = 0;
        char c1 = GetCell(r, 0);
        char c2 = GetCell(r, 1);
        if (c1 != ' ' || c2 != ' ') {
            // Crude parse: col 1 stores score as chars
            existingScore = static_cast<int>(c2 - '0') * 1000
                          + static_cast<int>(c1 - '0') * 100;
        }

        if (score > existingScore) {
            // Shift rows down
            for (int rr = num_rows_ - 1; rr > r; --rr) {
                for (int cc = 0; cc < num_cols_; ++cc) {
                    SetCell(rr, cc, GetCell(rr - 1, cc));
                }
            }
            // Write new entry
            SetCell(r, 0, name.empty() ? 'P' : name[0]);
            int s = std::min(score, 9999);
            SetCell(r, 1, static_cast<char>('0' + (s / 100) % 10));
            return;
        }
    }
}

std::vector<HighScoreEntry> HighScoreTable::getTopEntries(int count) const {
    std::vector<HighScoreEntry> result;
    for (int r = 0; r < std::min(count, num_rows_); ++r) {
        char nameChar = GetCell(r, 0);
        if (nameChar == ' ') break;
        char scoreChar = GetCell(r, 1);
        int score = static_cast<int>(scoreChar - '0') * 100;
        std::string name(1, nameChar);
        result.push_back({name, score});
    }
    return result;
}
