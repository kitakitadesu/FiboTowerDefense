#include "scoreboard.hpp"

#include <algorithm>
#include <fstream>

void Scoreboard::addScore(const std::string& name, int s) {
    entries_.push_back({name, s});
    std::sort(entries_.begin(), entries_.end(),
              [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
}

std::vector<ScoreEntry> Scoreboard::getTopScores(int count) const {
    std::vector<ScoreEntry> top;
    for (int i = 0; i < std::min(count, static_cast<int>(entries_.size())); ++i)
        top.push_back(entries_[i]);
    return top;
}

void Scoreboard::save(const std::string& filename) const {
    if (auto f = std::ofstream(filename)) {
        for (const auto& e : entries_)
            f << e.name << ":" << e.score << "\n";
    }
}

void Scoreboard::load(const std::string& filename) {
    entries_.clear();
    if (auto f = std::ifstream(filename)) {
        std::string line;
        while (std::getline(f, line)) {
            auto sep = line.find(':');
            if (sep != std::string::npos) {
                std::string name = line.substr(0, sep);
                try {
                    int s = std::stoi(line.substr(sep + 1));
                    entries_.push_back({name, s});
                } catch (...) { /* skip malformed */ }
            }
        }
        std::sort(entries_.begin(), entries_.end(),
                  [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
    }
}
