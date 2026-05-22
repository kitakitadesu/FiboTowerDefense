#pragma once

#include "raylib-cpp.hpp"

/// A placable solar cell that generates passive gold income. Upgradable to level 5.
class SolarCell {
public:
    explicit SolarCell(int col, int row);

    /// Advance timer. Returns gold earned this tick (0 or getReward()).
    int update(float dt);

    void draw(const raylib::Texture* tex, raylib::Vector2 screenPos) const;

    int getCol() const { return m_col; }
    int getRow() const { return m_row; }

    // --- level / upgrade ---
    static constexpr int kCost      = 60;
    static constexpr int kMaxLevel  = 5;

    int   getLevel()       const { return m_level; }
    bool  isMaxLevel()     const { return m_level >= kMaxLevel; }
    int   getUpgradeCost() const { return kCost * m_level; }
    int   getSellRefund()  const { return kCost * m_level / 2; }
    void  upgrade()              { if (m_level < kMaxLevel) ++m_level; }

    // --- level-dependent stats ---
    int   getReward()   const { return 6 + (m_level - 1) * 3; }   // 6, 9, 12, 15, 18
    float getInterval() const { return 1.8f - (m_level - 1) * 0.2f; }  // 1.8, 1.6, 1.4, 1.2, 1.0

private:
    int   m_col, m_row;
    int   m_level = 1;
    float m_timer = 0.0f;
};
