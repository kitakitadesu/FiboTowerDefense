#include "projectile.hpp"
#include "enemy.hpp"

#include <cmath>

Projectile::Projectile(raylib::Vector2 start, Enemy* target, float speed, int damage)
    : id_(IdGenerator::getNextId()), pos_(start), target_(target), speed_(speed), damage_(damage)
{
    // Compute initial direction toward target (straight line, no homing)
    if (target) {
        const raylib::Vector2 toTarget = target->getPosition() - start;
        const float d = toTarget.Length();
        if (d > 0.001f) dir_ = toTarget / d;
    }
}

bool Projectile::update(float dt) {
    if (impacted_) return true;

    // Advance in straight line
    pos_ = pos_ + dir_ * speed_ * dt;

    // Check if near target (collision radius)
    if (target_ && target_->isAlive()) {
        const raylib::Vector2 diff = target_->getPosition() - pos_;
        if (diff.LengthSqr() < 256.0f) {  // 16^2
            target_->takeDamage(damage_);
            impacted_ = true;
            return true;
        }
    }

    // Impact when off-screen
    if (pos_.x > 2200.0f || pos_.x < -200.0f || pos_.y < -200.0f || pos_.y > 1200.0f) {
        impacted_ = true;
        return true;
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
