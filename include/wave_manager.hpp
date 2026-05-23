#pragma once

#include <memory>
#include <vector>

#include "enemy.hpp"
#include "id_generator.hpp"
#include "identifier.hpp"

class GameBoard;

/// Wave definition: what to spawn and how fast.
struct WaveDef {
    int     enemyCount    = 5;
    float   spawnInterval = 1.5f;
    int     hp            = 100;
    float   speed         = 60.0f;
    int     reward        = 50;
};

/// Manages wave progression and enemy spawning along lane rows.
class WaveManager : public IIdentifier {
public:
    explicit WaveManager(const GameBoard& board);

    int  getId() const override { return id_; }

    void addWave(const WaveDef& def);
    void start();

    std::unique_ptr<Enemy> update(float dt);

    bool isWaveActive()  const;
    bool allWavesDone()  const;

    int  getCurrentWave() const { return currentWave_; }
    int  getWaveCount()   const { return static_cast<int>(waves_.size()); }
    int  getCurrentWaveEnemyCount() const;

    void advanceWave();

private:
    int id_;
    const GameBoard& board_;
    std::vector<WaveDef> waves_;
    int  currentWave_ = 0;
    int  spawned_     = 0;
    float timer_      = 0.0f;
    bool  started_    = false;
    bool  waiting_    = false;

    int  pickRow() const;

    int     infEnemyCount() const;
    int     infHp()    const;
    float   infSpeed() const;
    int     infReward() const;
    float   infInterval() const;
};
