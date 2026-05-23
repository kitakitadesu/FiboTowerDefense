#include "turret.hpp"

#include "enemy.hpp"
#include "projectile.hpp"

#include <algorithm>
#include <cmath>
#include <string>

Turret::Turret(int col, int row, TurretType type, float range, float fireRate, int damage)
    : id_(IdGenerator::getNextId()), col_(col), row_(row), type_(type),
      baseRange_(range), baseFireRate_(fireRate), baseDamage_(damage)
{
        if (type_ == TurretType::UR3e) {
        hp_ = 80.0f;
    } else {
        hp_ = 200.0f;
    }
    maxHp_ = hp_;
}

void Turret::takeDamage(float dmg) {
    hp_ = std::max(0.0f, hp_ - dmg);
}

void Turret::upgrade() {
    if (level_ >= kMaxLevel) return;
    ++level_;
    const float hpGain = (type_ == TurretType::UR3e) ? 20.0f : 40.0f;
    maxHp_ += hpGain;
    hp_ = maxHp_;
}

int Turret::getUpgradeCost() const {
    if (level_ >= kMaxLevel) return 0;
    const int baseCost = (type_ == TurretType::UR3e) ? 100 : 80;
    return baseCost * level_;
}

std::unique_ptr<Projectile> Turret::update(float dt, raylib::Vector2 screenPos,
                                            const std::vector<Enemy*>& enemies) {
    if (dead_ || hp_ <= 0.0f) return nullptr;

    Enemy* target = nullptr;
    float  nearest = 1e9f;
    const float effectiveRange = getRange();
    constexpr float kLeftTolerance = 60.0f;

    for (auto* e : enemies) {
        if (!e || !e->isAlive()) continue;
        if (e->getRow() != row_) continue;

        const float dx = e->getPosition().x - screenPos.x;

    if (type_ == TurretType::UR3e) {
            // Unlimited rightward, limited leftward (blocking enemies at turret)
            if (dx < -kLeftTolerance) continue;
            const float dist = std::abs(dx);
            if (dist < nearest) { nearest = dist; target = e; }
        } else {
            // Goose: symmetric range in both directions
            const float dist = std::abs(dx);
            if (dist > effectiveRange) continue;
            if (dist < nearest) { nearest = dist; target = e; }
        }
    }

    cooldown_ -= dt;
    if (cooldown_ > 0.0f) return nullptr;

    if (!target) { cooldown_ = 0.0f; return nullptr; }

    cooldown_ = 1.0f / getFireRate();
    return std::make_unique<Projectile>(screenPos, target, 300.0f, getDamage());
}

void Turret::draw(const raylib::Texture* tex, raylib::Vector2 screenPos, float cellW, float cellH, bool isNight) const {
    if (hp_ <= 0.0f) return;
    // Fit sprite inside cell preserving aspect ratio, with 10% padding
    const float s = std::min(cellW, cellH) * 0.85f;
    const float h = s / 2;

    if (tex) {
        const Rectangle src{0, 0, static_cast<float>(tex->width),
                                   static_cast<float>(tex->height)};
        const Rectangle dst{screenPos.x - h, screenPos.y - h, s, s};

        tex->Draw(src, dst, {}, 0.0f, raylib::Color::White());

        // Goose blue glow overlay at night (smooth radial gradient)
        if (isNight && type_ == TurretType::Goose) {
            DrawCircleGradient(screenPos, s * 0.75f,
                               {120, 200, 255, 102}, {120, 200, 255, 0});
        }
    } else {
        const raylib::Color c = (type_ == TurretType::UR3e) ? BLUE : ORANGE;
        DrawRectangle(static_cast<int>(screenPos.x - h),
                      static_cast<int>(screenPos.y - h),
                      static_cast<int>(s), static_cast<int>(s), c);
    }

    // Level badge
    {
        const std::string lvlStr = std::to_string(level_);
        const int lf = 12;
        const int lw = MeasureText(lvlStr.c_str(), lf);
        const float badgeX = screenPos.x - lw / 2.0f;
        const float badgeY = screenPos.y - h - 16;
        DrawRectangle(static_cast<int>(badgeX - 4), static_cast<int>(badgeY - 2),
                      lw + 8, 16, {30, 30, 40, 200});
        DrawText(lvlStr.c_str(), static_cast<int>(badgeX), static_cast<int>(badgeY), lf, WHITE);
    }

    // HP bar
    const float barW = 40.0f;
    const float barH = 4.0f;
    const float barX = screenPos.x - barW / 2;
    const float barY = screenPos.y - h - 8;
    const float ratio = hp_ / maxHp_;

    DrawRectangle(static_cast<int>(barX - 1), static_cast<int>(barY - 1),
                  static_cast<int>(barW + 2), static_cast<int>(barH + 2), {30, 30, 40, 200});
    raylib::Color hpColor = (ratio > 0.5f) ? GREEN : ((ratio > 0.25f) ? ORANGE : RED);
    DrawRectangle(static_cast<int>(barX), static_cast<int>(barY),
                  static_cast<int>(barW * ratio), static_cast<int>(barH), hpColor);
}
