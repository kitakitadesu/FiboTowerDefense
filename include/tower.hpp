#pragma once

#include <vector>

#include "raylib-cpp.hpp"

class Enemy;
class Projectile;

/// A placed tower that auto-attacks enemies in its row.
class Tower {
public:
    Tower(int col, int row, float range, float fireRate, int damage);

    /// Update cooldown. Fire at nearest enemy in row within range. Returns new projectile or null.
    Projectile* update(float dt, raylib::Vector2 screenPos, const std::vector<Enemy*>& enemies);

    void draw(const raylib::Texture* tex, raylib::Vector2 screenPos) const;

    bool isDead() const { return dead_; }

    int getCol() const { return col_; }
    int getRow() const { return row_; }
    int getDamage() const { return damage_; }

private:
    int   col_, row_;
    float range_;
    float fireRate_;   ///< shots per second
    float cooldown_ = 0.0f;
    int   damage_;
    bool  dead_ = false;
};
