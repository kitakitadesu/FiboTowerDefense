#pragma once

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

    void addScore(const std::string& name, int s);
    void addPoints(int pts) { currentScore_ += pts; }
    int  getCurrentScore() const { return currentScore_; }

    std::vector<ScoreEntry> getTopScores(int count) const;
    void save(const std::string& filename) const;
    void load(const std::string& filename);

private:
    int id_;
    int currentScore_ = 0;
    std::vector<ScoreEntry> entries_;
};
