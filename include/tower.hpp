#pragma once

#include <string>

#include "raylib-cpp.hpp"

#include "id_generator.hpp"
#include "identifier.hpp"
#include "idisplay_name.hpp"
#include "isprite.hpp"

/// Placeholder struct — expanded in future phases (boss integration).
struct TowerData {};

/// The base to defend. When destroyed → game over.
class Tower : public IIdentifier, public IDisplayName, public ISprite {
public:
    explicit Tower(int maxHp);

    int getId() const override { return id_; }
    std::string getDisplayName() const override { return "Main Base"; }

    // ISprite
    void draw() const override;

    // HP
    int  getHp()    const { return hp_; }
    int  getMaxHp() const { return maxHp_; }
    void setHp(int hp) { hp_ = std::max(0, hp); }

    // Shield
    bool hasShield() const { return shield_; }
    void setShield(bool s) { shield_ = s; }

    /// Shield absorbs fully while active; otherwise reduces HP.
    void takeDamage(int amount);
    bool isDestroyed() const { return hp_ <= 0; }

    const TowerData& getTowerData() const { return data_; }

private:
    int       id_;
    int       hp_;
    int       maxHp_;
    bool      shield_ = false;
    TowerData data_;
};
