#pragma once

#include <memory>
#include <vector>

#include "enemy.hpp"

class Board;

/// Wave definition: what to spawn and how fast.
struct WaveDef {
    int     enemyCount    = 5;
    float   spawnInterval = 1.5f;
    int     hp            = 100;
    float   speed         = 60.0f;
    int     reward        = 50;
};

/// Manages wave progression and enemy spawning along lane rows.
class WaveManager {
public:
    explicit WaveManager(const Board& board);

    void addWave(const WaveDef& def);
    void start();

    /// Returns newly spawned enemy or nullptr.
    std::unique_ptr<Enemy> update(float dt);

    bool isWaveActive()  const;  ///< wave spawned all but enemies still alive
    bool allWavesDone()  const;  ///< all waves completed

    int  getCurrentWave() const { return currentWave_; }
    int  getWaveCount()   const { return static_cast<int>(waves_.size()); }
    int  getSpawnedCount() const { return spawned_; }
    int  getCurrentWaveEnemyCount() const;

    void setInfiniteMode(bool on) { infinite_ = on; }
    bool isInfinite()       const { return infinite_; }

    /// Call when current wave enemies are all dead/escaped to advance.
    void advanceWave();
    void markWaiting(bool w);

private:
    const Board& board_;
    std::vector<WaveDef> waves_;
    int  currentWave_ = 0;
    int  spawned_     = 0;
    float timer_      = 0.0f;
    bool  started_    = false;
    bool  waiting_    = false;
    bool  infinite_   = false;

    int  pickRow() const;

    /// Compute wave params for infinite waves beyond predefined list.
    int     infiniteEnemyCount() const;
    int     infiniteHp()    const;
    float   infiniteSpeed() const;
    int     infiniteReward() const;
    float   infiniteInterval() const;
};
