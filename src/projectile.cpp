#include "projectile.hpp"
#include "enemy.hpp"

#include <cmath>

Projectile::Projectile(raylib::Vector2 start, Enemy* target, float speed, int damage)
    : id_(IdGenerator::getNextId()), pos_(start), target_(target), speed_(speed), damage_(damage)
{}

bool Projectile::update(float dt) {
    if (impacted_) return true;

    // Free-flying after target lost — continue in last direction
    if (flying_) {
        pos_ = pos_ + dir_ * speed_ * dt;
        // Impact when off-screen (past right edge or far left)
        if (pos_.x > 2200.0f || pos_.x < -200.0f) {
            impacted_ = true;
            return true;
        }
        return false;
    }

    if (!target_ || !target_->isAlive()) {
        // Target died — save direction and keep flying
        flying_ = true;
        return false;
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
        dir_ = diff / dist;
        pos_ = pos_ + dir_ * step;
    }

    return false;
}

void Projectile::draw(bool isNight) const {
    if (impacted_) return;
    if (isNight) {
        // Night: larger yellow gradient glow
        DrawCircleGradient(pos_, 14, {255, 255, 150, 120}, {255, 255, 0, 0});
        DrawCircleGradient(pos_, 6,  {255, 255, 200, 200}, {255, 255, 200, 0});
    } else {
        // Day: smaller yellow gradient glow
        DrawCircleGradient(pos_, 10, {255, 255, 150, 100}, {255, 255, 0, 0});
        DrawCircleGradient(pos_, 4,  {255, 255, 200, 200}, {255, 255, 200, 0});
    }
}
