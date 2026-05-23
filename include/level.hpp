#pragma once

#include <raylib.h> 
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "board.hpp"
#include "enemy.hpp"
#include "id_generator.hpp"
#include "identifier.hpp"
#include "projectile.hpp"
#include "scoreboard.hpp"
#include "solar_cell.hpp"
#include "tower.hpp"
#include "turret.hpp"
#include "util.hpp"
#include "wave_manager.hpp"

/// Build mode — what the player is placing on next click.
enum class BuildMode {
    None,
    ShootTurret,
    MeleeTurret,
    SolarCell
};

struct EnemyKilledEvent {
    raylib::Vector2 position;
};

/// Floating wave-announcement banner state.
struct WaveAnnouncement {
    std::string text;
    float life = 3.0f;
    float maxLife = 3.0f;
};

/// Container for all gameplay entities in one level.
class Level : public IIdentifier {
public:
    Level(Board& grid, Tower& tower, Scoreboard& scoreboard);

    ~Level();

    int getId() const override { return id_; }

    // --- accessors ---
    Board&       getGrid()       { return grid_; }
    const Board& getGrid() const { return grid_; }
    Tower&       getTower()             { return tower_; }
    const Tower& getTower()       const { return tower_; }

    std::vector<Turret>&               getTurrets()       { return turrets_; }
    std::vector<std::unique_ptr<Enemy>>& getEnemies()       { return enemies_; }
    std::vector<std::unique_ptr<Projectile>>& getProjectiles() { return projectiles_; }
    std::vector<SolarCell>&            getSolarCells()    { return solarCells_; }
    std::vector<WaveManager>&          getWaves()         { return waves_; }
    WaveManager&                       getWaveManager()   { return waveMgr_; }
    Scoreboard&                        getScoreboard()    { return scoreboard_; }

    // --- currency ---
    int  getCurrency()           const { return currency_; }
    void setCurrency(int c)           { currency_ = c; }
    void addCurrency(int amt)         { currency_ += amt; }
    bool spendCurrency(int amt);
    float getCurrencyMultiplier()           const { return currencyMult_; }
    void  setCurrencyMultiplier(float m)          { currencyMult_ = m; }

    // --- queries ---
    bool isTowerDestroyed() const { return tower_.isDestroyed(); }
    int  getEnemiesKilledThisWave() const { return enemiesKilledThisWave_; }
    int  getTotalEnemiesThisWave()  const { return totalEnemiesThisWave_; }
    int  getTurretsPlaced()   const { return turretsPlaced_; }
    int  getSolarPlaced()     const { return solarPlaced_; }
    bool getCheatMode()       const { return cheatMode_; }
    int  getSelectedTurretIdx() const { return selectedTurretIdx_; }
    int  getSelectedSolarIdx()  const { return selectedSolarIdx_; }
    void setSelectedTurretIdx(int i) { selectedTurretIdx_ = i; }
    void setSelectedSolarIdx(int i)  { selectedSolarIdx_ = i; }
    void setCheatMode(bool c)       { cheatMode_ = c; }
    void setPaused(bool p)          { paused_ = p; }

    std::vector<FloatingText>& getFloatingTexts() { return floatingTexts_; }

    // --- add helpers ---
    void addTurret(const Turret& t)  { turrets_.push_back(t); ++turretsPlaced_; }
    void addEnemy(std::unique_ptr<Enemy> e);
    void addSolarCell(const SolarCell& s) { solarCells_.push_back(s); ++solarPlaced_; }

    // --- lifecycle ---
    void start();
    void update(float dt, const std::vector<std::vector<raylib::Vector2>>& laneWps);
    void render(const raylib::Texture* gooseRaw, const Texture2D* nightMap = nullptr, float nightAlpha = 0.0f);
    void renderUI();

    // --- events ---
    entt::dispatcher& events() { return events_; }
    void listenToEvents();

    // --- stubs for Phase 3+ ---
    void removeTurret(int x, int y) { (void)x; (void)y; }
    void updateBossAttacks(float) {}

private:
    int id_;
    Board& grid_;
    Tower& tower_;
    Scoreboard& scoreboard_;

    std::vector<Turret> turrets_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Projectile>> projectiles_;
    std::vector<WaveManager> waves_;
    std::vector<SolarCell> solarCells_;

    int   currency_ = 300;
    float currencyMult_ = 1.0f;

    WaveManager waveMgr_;
    entt::dispatcher events_;

    int enemiesKilledThisWave_ = 0;
    int totalEnemiesThisWave_  = 0;
    std::vector<FloatingText> floatingTexts_;
    int selectedTurretIdx_ = -1;
    int selectedSolarIdx_  = -1;
    int turretsPlaced_     = 0;
    int solarPlaced_       = 0;
    bool cheatMode_ = false;
    bool paused_ = false;
    BuildMode placingMode_ = BuildMode::None;

    Sound sfxSelect_;
    Sound sfxPlace_;
    Sound sfxError_;

    // Wave announcement
    std::optional<WaveAnnouncement> waveAnnounce_;

    // Sell confirm
    int sellConfirmIdx_ = -1;
    bool sellConfirmIsTurret_ = true;
    int sellConfirmGold_ = 0;
    float sellConfirmTimer_ = 0.0f;
    int sellConfirmSelCol_ = 0;
    int sellConfirmSelRow_ = 0;
};
