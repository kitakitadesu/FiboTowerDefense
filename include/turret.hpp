#pragma once

#include <memory>
#include <string>
#include <vector>

#include "raylib-cpp.hpp"

#include "id_generator.hpp"
#include "identifier.hpp"
#include "idisplay_name.hpp"
#include "isprite.hpp"

class Enemy;
class Projectile;

/// Turret type: UR3e (ranged, unlimited range rightward) vs Goose (short range, tanky).
enum class TurretType {
    UR3e,
    Goose
};

/// A placed turret that auto-attacks enemies in its row. Upgradable to level 5.
class Turret : public IIdentifier, public IDisplayName, public ISprite {
public:
    Turret(int col, int row, TurretType type, float range, float fireRate, int damage);

    int getId() const override { return id_; }
    std::string getDisplayName() const override {
        return (type_ == TurretType::UR3e) ? "UR3e" : "Goose";
    }

    /// ISprite::draw() — lightweight stub (position-dependent draw uses overload below).
    void draw() const override {}

    /// Update cooldown. Fire at nearest enemy in row within range. Returns new projectile or null.
    std::unique_ptr<Projectile> update(float dt, raylib::Vector2 screenPos, const std::vector<Enemy*>& enemies);

    void draw(const raylib::Texture* tex, raylib::Vector2 screenPos, float cellW, float cellH, bool isNight) const;

    void takeDamage(float dmg);

    bool isDestroyed() const { return hp_ <= 0.0f; }
    bool isAlive()    const { return hp_ > 0.0f; }

    int getCol() const { return col_; }
    int getRow() const { return row_; }

    // --- stats (scale with level) ---
    int   getLevel()    const { return level_; }
    int   getDamage()   const { return baseDamage_  + (level_ - 1) * 10; }
    float getFireRate() const { return baseFireRate_ + (level_ - 1) * 0.1f; }
    float getHp()       const { return hp_; }
    float getMaxHp()    const { return maxHp_; }
    TurretType getTurretType() const { return type_; }
    float getRange() const {
        return (type_ == TurretType::UR3e) ? 9999.0f
                                           : baseRange_ + (level_ - 1) * 20.0f;
    }

    // --- upgrade ---
    static constexpr int kMaxLevel = 5;
    bool isMaxLevel() const { return level_ >= kMaxLevel; }
    int  getUpgradeCost() const;
    void upgrade();

private:
    int    id_;
    int    col_, row_;
    TurretType type_;
    float  baseRange_;
    float  baseFireRate_;
    int    baseDamage_;
    float  cooldown_ = 0.0f;
    float  hp_;
    float  maxHp_;
    bool   dead_ = false;
    int    level_ = 1;
};
