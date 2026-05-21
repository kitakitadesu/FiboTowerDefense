#include "wave_manager.hpp"
#include "board.hpp"

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

Enemy* WaveManager::update(float dt) {
    if (!started_ || currentWave_ >= static_cast<int>(waves_.size()))
        return nullptr;

    if (waiting_) return nullptr; // wait for external signal

    const WaveDef& wave = waves_[currentWave_];
    timer_ -= dt;

    if (timer_ <= 0.0f && spawned_ < wave.enemyCount) {
        // Spawn
        const int row = pickRow();
        auto* enemy = new Enemy(row, wave.hp, wave.speed, wave.reward);
        ++spawned_;
        timer_ = wave.spawnInterval;
        return enemy;
    }

    // If all spawned, caller tells us when wave is done
    return nullptr;
}

bool WaveManager::isWaveActive() const {
    return started_ && currentWave_ < static_cast<int>(waves_.size()) &&
           spawned_ > 0 && spawned_ >= waves_[currentWave_].enemyCount;
}

bool WaveManager::allWavesDone() const {
    return started_ && currentWave_ >= static_cast<int>(waves_.size());
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
