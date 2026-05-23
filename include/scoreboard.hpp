#pragma once

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "id_generator.hpp"
#include "identifier.hpp"

struct ScoreEntry {
    std::string name;
    int score;
};

/// Tracks current score and persistent high scores.
class Scoreboard : public IIdentifier {
public:
    Scoreboard() : id_(IdGenerator::getNextId()) {}

    int getId() const override { return id_; }

    void addScore(const std::string& name, int s) {
        entries_.push_back({name, s});
        std::sort(entries_.begin(), entries_.end(),
                  [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
    }

    void addPoints(int pts) { currentScore_ += pts; }
    int  getCurrentScore() const { return currentScore_; }

    std::vector<ScoreEntry> getTopScores(int count) const {
        std::vector<ScoreEntry> top;
        for (int i = 0; i < std::min(count, static_cast<int>(entries_.size())); ++i)
            top.push_back(entries_[i]);
        return top;
    }

    /// Format: one "name:score" per line.
    void save(const std::string& filename) const {
        if (auto f = std::ofstream(filename)) {
            for (const auto& e : entries_)
                f << e.name << ":" << e.score << "\n";
        }
    }

    void load(const std::string& filename) {
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
                    } catch (...) { /* skip malformed line */ }
                }
            }
            std::sort(entries_.begin(), entries_.end(),
                      [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
        }
    }

private:
    int id_;
    int currentScore_ = 0;
    std::vector<ScoreEntry> entries_;
};
