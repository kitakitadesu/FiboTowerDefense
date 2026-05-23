#pragma once

#include "raylib-cpp.hpp"

/// HP component for damageable entities (composition over inheritance).
class Health {
public:
    Health(float maxHp) : hp_(maxHp), maxHp_(maxHp) {}

    void takeDamage(float dmg) { hp_ -= dmg; }
    bool isAlive()       const { return hp_ > 0.0f; }
    bool isDead()        const { return hp_ <= 0.0f; }
    float getHp()        const { return hp_; }
    float getMaxHp()     const { return maxHp_; }
    float getRatio()     const { return maxHp_ > 0.0f ? hp_ / maxHp_ : 0.0f; }
    void  fill()               { hp_ = maxHp_; }
    void  setMaxHp(float m)    { maxHp_ = m; hp_ = m; }

private:
    float hp_;
    float maxHp_;
};

/// A placable solar cell that generates passive gold income. Upgradable to level 5.
class SolarCell {
public:
    explicit SolarCell(int col, int row);

    /// Advance timer. Returns gold earned this tick (0 or getReward()). No energy at night.
    int update(float dt, bool isNight);

    void draw(const raylib::Texture* tex, raylib::Vector2 screenPos, float cellW, float cellH) const;

    int getCol() const { return m_col; }
    int getRow() const { return m_row; }

    // --- level / upgrade ---
    static constexpr int kCost      = 60;
    static constexpr int kMaxLevel  = 5;

    int   getLevel()       const { return m_level; }
    bool  isMaxLevel()     const { return m_level >= kMaxLevel; }
    int   getUpgradeCost() const { return kCost * m_level; }
    int   getSellRefund()  const { return kCost * m_level / 2; }
    void  upgrade()              { if (m_level < kMaxLevel) { ++m_level; health_.setMaxHp(20.0f + (m_level - 1) * 10.0f); health_.fill(); } }

    // --- level-dependent stats ---
    int   getReward()   const { return 6 + (m_level - 1) * 3; }   // 6, 9, 12, 15, 18
    float getInterval() const { return 1.8f - (m_level - 1) * 0.2f; }  // 1.8, 1.6, 1.4, 1.2, 1.0

    // --- HP (delegated to Health component) ---
    void  takeDamage(float dmg) { health_.takeDamage(dmg); }
    bool  isDestroyed()   const { return health_.isDead(); }
    bool  isAlive()       const { return health_.isAlive(); }
    float getHp()         const { return health_.getHp(); }
    float getMaxHp()      const { return health_.getMaxHp(); }

private:
    int    m_col, m_row;
    int    m_level = 1;
    float  m_timer = 0.0f;
    Health health_{20.0f};
};
