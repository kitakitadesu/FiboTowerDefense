#include "wave_manager.hpp"
#include "board.hpp"

#include <algorithm>
#include <cstdlib>  // std::rand

WaveManager::WaveManager(const Board& board) : board_(board) {}

void WaveManager::addWave(const WaveDef& def) {
    waves_.push_back(def);
}

void WaveManager::start() {
    currentWave_ = 0;
    spawned_     = 0;
    timer_       = 0.0f;
    started_     = true;
    waiting_     = false;
}

std::unique_ptr<Enemy> WaveManager::update(float dt) {
    if (!started_) return nullptr;

    // Past predefined waves and not infinite → stop spawning
    if (!infinite_ && currentWave_ >= static_cast<int>(waves_.size()))
        return nullptr;

    if (waiting_) return nullptr; // wait for external signal

    timer_ -= dt;
    if (timer_ > 0.0f) return nullptr;

    const int targetCount = getCurrentWaveEnemyCount();
    if (spawned_ < targetCount) {
        const int row = pickRow();
        ++spawned_;
        if (currentWave_ < static_cast<int>(waves_.size())) {
            // Predefined wave
            const WaveDef& w = waves_[currentWave_];
            timer_ = w.spawnInterval;
            return std::make_unique<Enemy>(row, w.hp, w.speed, w.reward);
        }
        // Infinite wave
        timer_ = infiniteInterval();
        return std::make_unique<Enemy>(row, infiniteHp(), infiniteSpeed(), infiniteReward());
    }

    return nullptr;
}

bool WaveManager::isWaveActive() const {
    if (!started_) return false;
    // Any wave is active once all enemies spawned and not yet advanced
    return spawned_ > 0 && spawned_ >= getCurrentWaveEnemyCount();
}

bool WaveManager::allWavesDone() const {
    if (infinite_) return false;
    return started_ && currentWave_ >= static_cast<int>(waves_.size());
}

int WaveManager::getCurrentWaveEnemyCount() const {
    if (currentWave_ < static_cast<int>(waves_.size()))
        return waves_[currentWave_].enemyCount;
    if (infinite_) return infiniteEnemyCount();
    return 0;
}

int WaveManager::pickRow() const {
    return std::rand() % board_.getRowCount();
}

// Called externally to advance to next wave or finish
void WaveManager::advanceWave() {
    ++currentWave_;
    spawned_ = 0;
    timer_   = 3.0f; // 3 second delay before next wave
}

void WaveManager::markWaiting(bool w) {
    waiting_ = w;
}

// ── Infinite wave scaling (ramps from wave 5 / index 4 onward) ──
int   WaveManager::infiniteEnemyCount() const { return 20 + (currentWave_ - 4) * 3; }
int   WaveManager::infiniteHp()    const { return 170 + (currentWave_ - 4) * 40; }
float WaveManager::infiniteSpeed() const { return 70.0f + (currentWave_ - 4) * 3.0f; }
int   WaveManager::infiniteReward() const { return 52 + (currentWave_ - 4) * 5; }
float WaveManager::infiniteInterval() const {
    return std::max(0.3f, 0.8f - (currentWave_ - 4) * 0.02f);
}
