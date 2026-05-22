#include "tower.hpp"

#include <algorithm>
#include <string>

Tower::Tower(int maxHp)
    : id_(IdGenerator::getNextId()), hp_(maxHp), maxHp_(maxHp)
{}

void Tower::takeDamage(int amount) {
    if (shield_ || hp_ <= 0) return;  // shield absorbs all, or already dead
    hp_ = std::max(0, hp_ - amount);
}

void Tower::draw() const {
    // Building-shaped base on the left side of the screen
    const int scrH = GetScreenHeight();
    const int bw = 80;
    const int bh = 90;
    const int bx = 30;
    const int by = scrH / 2 - bh / 2;

    // Body
    DrawRectangleRounded({static_cast<float>(bx), static_cast<float>(by),
                          static_cast<float>(bw), static_cast<float>(bh)},
                         0.15f, 8, {60, 60, 80, 255});
    // Roof
    DrawTriangle({static_cast<float>(bx - 8), static_cast<float>(by)},
                 {static_cast<float>(bx + bw / 2), static_cast<float>(by - 30)},
                 {static_cast<float>(bx + bw + 8), static_cast<float>(by)},
                 {100, 100, 140, 255});
    // Door
    DrawRectangleRounded({static_cast<float>(bx + bw / 2 - 12), static_cast<float>(by + bh - 35),
                          24.0f, 30.0f}, 0.3f, 6, {140, 110, 60, 255});

    // Shield overlay
    if (shield_) {
        DrawRectangleRoundedLines({static_cast<float>(bx - 4), static_cast<float>(by - 4),
                                   static_cast<float>(bw + 8), static_cast<float>(bh + 8)},
                                  0.15f, 8, {100, 200, 255, 200});
    }

    // HP bar above
    const float barW = static_cast<float>(bw);
    const float barH = 6.0f;
    const float barX = static_cast<float>(bx);
    const float barY = static_cast<float>(by - 16);
    const float ratio = static_cast<float>(hp_) / static_cast<float>(maxHp_);

    DrawRectangleRounded({barX - 1, barY - 1, barW + 2, barH + 2}, 0.5f, 4, {20, 20, 20, 200});
    raylib::Color hpColor = (ratio > 0.5f) ? GREEN : ((ratio > 0.25f) ? ORANGE : RED);
    DrawRectangleRounded({barX, barY, barW * ratio, barH}, 0.5f, 4, hpColor);

    // HP text
    const std::string hpStr = std::to_string(hp_) + "/" + std::to_string(maxHp_);
    const int fs = 12;
    const int tw = MeasureText(hpStr.c_str(), fs);
    DrawText(hpStr.c_str(), bx + bw / 2 - tw / 2, by - 28, fs, LIGHTGRAY);
}
