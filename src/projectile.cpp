#include "projectile.hpp"
#include "enemy.hpp"

#include <cmath>

Projectile::Projectile(raylib::Vector2 start, Enemy* target, float speed, int damage)
    : id_(IdGenerator::getNextId()), pos_(start), target_(target), speed_(speed), damage_(damage)
{}

bool Projectile::update(float dt) {
    if (impacted_) return true;
    if (!target_ || !target_->isAlive()) {
        impacted_ = true;
        return true;
    }

    const raylib::Vector2 diff = target_->getPosition() - pos_;
    const float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist < 16.0f) {
        // Hit
        target_->takeDamage(damage_);
        impacted_ = true;
        return true;
    }

    const float step = speed_ * dt;
    if (dist <= step) {
        pos_ = target_->getPosition();
    } else {
        pos_ = pos_ + (diff / dist * step);
    }

    return false;
}

void Projectile::draw(bool isNight) const {
    if (impacted_) return;
    if (isNight) {
        // Night: brighter, larger yellow glow
        DrawCircle(static_cast<int>(pos_.x), static_cast<int>(pos_.y), 12, {255, 255, 0, 120});
        DrawCircle(static_cast<int>(pos_.x), static_cast<int>(pos_.y), 5, {255, 255, 150, 255});
    } else {
        // Day: normal glow
        DrawCircle(static_cast<int>(pos_.x), static_cast<int>(pos_.y), 8, {255, 255, 0, 100});
        DrawCircle(static_cast<int>(pos_.x), static_cast<int>(pos_.y), 4, {255, 255, 200, 255});
    }
}
