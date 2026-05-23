#pragma once

#include "raylib-cpp.hpp"

#include "id_generator.hpp"
#include "identifier.hpp"

class Enemy;

/// Simple projectile fired from a tower toward an enemy.
class Projectile : public IIdentifier {
public:
    Projectile(raylib::Vector2 start, Enemy* target, float speed, int damage);

    int getId() const override { return id_; }

    /// Returns true when projectile has hit the target.
    bool update(float dt);

    void draw() const;

    bool hasImpacted() const { return impacted_; }

private:
    int id_;
    raylib::Vector2 pos_;
    Enemy*          target_ = nullptr;
    float           speed_;
    int             damage_;
    bool            impacted_ = false;
};
