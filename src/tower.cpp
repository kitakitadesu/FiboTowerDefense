#include "tower.hpp"

#include <algorithm>

Tower::Tower(int maxHp)
    : id_(IdGenerator::getNextId()), hp_(maxHp), maxHp_(maxHp)
{}

void Tower::takeDamage(int amount) {
    if (hp_ <= 0) return;
    hp_ = std::max(0, hp_ - amount);
}
