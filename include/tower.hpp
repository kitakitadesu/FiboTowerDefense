#pragma once

#include <string>

#include "raylib-cpp.hpp"

#include "id_generator.hpp"
#include "identifier.hpp"

/// The base to defend. When destroyed → game over.
class Tower : public IIdentifier {
public:
    explicit Tower(int maxHp);

    int getId() const override { return id_; }

    int  getHp()    const { return hp_; }
    int  getMaxHp() const { return maxHp_; }

    void takeDamage(int amount);
    bool isDestroyed() const { return hp_ <= 0; }

private:
    int       id_;
    int       hp_;
    int       maxHp_;
};
