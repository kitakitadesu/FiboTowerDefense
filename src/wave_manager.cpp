#include "wave_manager.hpp"
#include "game_board.hpp"

#include <algorithm>
#include <random>

WaveManager::WaveManager(const GameBoard& board)
    : id_(IdGenerator::getNextId()), board_(board) {}

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

    if (waiting_) return nullptr; // wait for external signal

    timer_ -= dt;
    if (timer_ > 0.0f) return nullptr;

    const int targetCount = getCurrentWaveEnemyCount();
    if (spawned_ < targetCount) {
        const int row = pickRow();
        ++spawned_;
        if (currentWave_ < static_cast<int>(waves_.size())) {
            const WaveDef& w = waves_[currentWave_];
            timer_ = w.spawnInterval;
            return std::make_unique<Enemy>(row, w.hp, w.speed, w.reward);
        }
        // Past predefined waves — infinite scaling
        timer_ = infInterval();
        return std::make_unique<Enemy>(row, infHp(), infSpeed(), infReward());
    }

    return nullptr;
}

bool WaveManager::isWaveActive() const {
    if (!started_) return false;
    return spawned_ > 0 && spawned_ >= getCurrentWaveEnemyCount();
}

bool WaveManager::allWavesDone() const {
    return false; // infinite waves — game never ends in victory
}

int WaveManager::getCurrentWaveEnemyCount() const {
    if (currentWave_ < static_cast<int>(waves_.size()))
        return waves_[currentWave_].enemyCount;
    return infEnemyCount();
}

int WaveManager::pickRow() const {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, board_.getRowCount() - 1);
    return dist(rng);
}

// Called externally to advance to next wave or finish
void WaveManager::advanceWave() {
    ++currentWave_;
    spawned_ = 0;
    timer_   = 3.0f; // 3 second delay before next wave
}

// ── Infinite wave scaling (ramps from wave 5 / index 4 onward) ──
int   WaveManager::infEnemyCount() const { return 20 + (currentWave_ - 4) * 3; }
int   WaveManager::infHp()    const { return 170 + (currentWave_ - 4) * 40; }
float WaveManager::infSpeed() const { return 70.0f + (currentWave_ - 4) * 3.0f; }
int   WaveManager::infReward() const { return 52 + (currentWave_ - 4) * 5; }
float WaveManager::infInterval() const {
    return std::max(0.3f, 0.8f - (currentWave_ - 4) * 0.02f);
}
