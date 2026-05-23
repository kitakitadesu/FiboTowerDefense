#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <raylib.h>

#include <entt/entt.hpp>

#include "game_board.hpp"
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
#include "game_grid.hpp"

/// Build mode for placing turrets/solar cells.
enum class BuildMode { None, ShootTurret, MeleeTurret, SolarCell };

/// Floating wave-announcement banner.
struct WaveAnnouncement {
    std::string text;
    float life = 3.0f;
    float maxLife = 3.0f;
};

/// Event fired when an enemy is killed (not escaped).
struct EnemyKilledEvent {
    raylib::Vector2 position;
};

/// Container for all gameplay entities in one level.
class Level : public IIdentifier {
public:
    Level(GameBoard& grid, Tower& tower, Scoreboard& scoreboard);
    ~Level();

    int getId() const override { return id_; }

    // --- core accessors ---
    GameBoard&   getGrid()       { return grid_; }
    Tower&       getTower()             { return tower_; }
    const Tower& getTower()       const { return tower_; }
    Scoreboard&  getScoreboard()        { return scoreboard_; }
    int          getCurrency()    const { return currency_; }
    WaveManager& getWaveManager()       { return waveMgr_; }

    /// For win condition check (game.cpp).
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies_; }

    // --- queries ---
    bool isTowerDestroyed() const { return tower_.isDestroyed(); }
    bool getCheatMode()    const { return cheatMode_; }
    void setCheatMode(bool c)    { cheatMode_ = c; }
    void setPaused(bool p)       { paused_ = p; }

    // --- currency ---
    void addCurrency(int amt)         { currency_ += amt; }
    bool spendCurrency(int amt);

    // --- add helpers ---
    void addTurret(const Turret& t)  { turrets_.push_back(t); ++turretsPlaced_; }
    void addEnemy(std::unique_ptr<Enemy> e);
    void addSolarCell(const SolarCell& s) { solarCells_.push_back(s); ++solarPlaced_; }

    // --- lifecycle ---
    void start();
    void update(float dt, const std::vector<std::vector<raylib::Vector2>>& laneWps, bool isNight);
    void render(const raylib::Texture* ur3eTex, const raylib::Texture* gooseTex,
                const raylib::Texture* solarTex, const raylib::Texture* manTex,
                const Texture2D* nightMap = nullptr, float nightAlpha = 0.0f);
    void renderUI();

    // --- events ---
    entt::dispatcher& events() { return events_; }
    void onEnemyKilled(const EnemyKilledEvent& event);
    void listenToEvents();

    // --- stubs for Phase 3+ ---
    void removeTurret(int x, int y) { (void)x; (void)y; }
    void updateBossAttacks(float) {}
    void setTowerHitSound(Sound* s) { towerHitSound_ = s; }
private:

    int id_;
    GameBoard& grid_;
    Tower& tower_;
    Scoreboard& scoreboard_;

    entt::dispatcher events_;

    std::vector<Turret> turrets_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Projectile>> projectiles_;
    std::vector<SolarCell> solarCells_;

    PlacementGrid placements_{5, 9};

    int   currency_ = 300;
    WaveManager waveMgr_;

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

    Sound* towerHitSound_ = nullptr;
    Sound sfxSelect_;
    Sound sfxPlace_;
    Sound sfxError_;
    

    std::optional<WaveAnnouncement> waveAnnounce_;

    int sellConfirmIdx_ = -1;
    bool sellConfirmIsTurret_ = true;
    int sellConfirmGold_ = 0;
    float sellConfirmTimer_ = 0.0f;
    int sellConfirmSelCol_ = 0;
    int sellConfirmSelRow_ = 0;
};
