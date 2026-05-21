#include "tower.hpp"

#include "enemy.hpp"
#include "projectile.hpp"

#include <algorithm>
#include <cmath>

Tower::Tower(int col, int row, float range, float fireRate, int damage)
    : col_(col), row_(row), range_(range),
      fireRate_(fireRate), damage_(damage)
{}

Projectile* Tower::update(float dt, raylib::Vector2 screenPos,
                           const std::vector<Enemy*>& enemies) {
    if (dead_) return nullptr;

    // Find nearest alive enemy in same row within range
    Enemy* target = nullptr;
    float  nearest = range_ + 1.0f;

    for (auto* e : enemies) {
        if (!e || !e->isAlive()) continue;
        if (e->getRow() != row_) continue;

        const float dx = std::abs(e->getPosition().x - screenPos.x);
        if (dx <= range_ && dx < nearest) {
            nearest = dx;
            target  = e;
        }
    }

    // Cooldown
    cooldown_ -= dt;
    if (cooldown_ > 0.0f) return nullptr;

    if (!target) {
        cooldown_ = 0.0f;
        return nullptr;
    }

    // Fire!
    cooldown_ = 1.0f / fireRate_;
    return new Projectile(screenPos, target, 300.0f, damage_);
}

void Tower::draw(const raylib::Texture* tex, raylib::Vector2 screenPos) const {
    if (dead_) return;
    const float s = 50.0f;
    const float h = s / 2;

    if (tex) {
        const Rectangle src{0, 0, static_cast<float>(tex->width),
                                   static_cast<float>(tex->height)};
        const Rectangle dst{screenPos.x - h, screenPos.y - h, s, s};
        tex->Draw(src, dst, {}, 0.0f, raylib::Color::White());
    } else {
        DrawRectangle(static_cast<int>(screenPos.x - h),
                      static_cast<int>(screenPos.y - h),
                      static_cast<int>(s), static_cast<int>(s), BLUE);
    }
}
