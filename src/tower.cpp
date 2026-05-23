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
    DrawRectangle(bx, by, bw, bh, {35, 35, 50, 255});
    // Roof
    DrawTriangle({static_cast<float>(bx - 8), static_cast<float>(by)},
                 {static_cast<float>(bx + bw / 2), static_cast<float>(by - 30)},
                 {static_cast<float>(bx + bw + 8), static_cast<float>(by)},
                 {55, 55, 75, 255});
    // Door
    DrawRectangle(bx + bw / 2 - 12, by + bh - 35, 24, 30, {180, 120, 40, 255});

    // Shield overlay
    if (shield_) {
        DrawRectangleLinesEx({static_cast<float>(bx - 4), static_cast<float>(by - 4),
                              static_cast<float>(bw + 8), static_cast<float>(bh + 8)},
                             2.0f, {255, 140, 20, 200});
    }

    // HP bar above
    const float barW = static_cast<float>(bw);
    const float barH = 6.0f;
    const float barX = static_cast<float>(bx);
    const float barY = static_cast<float>(by - 16);
    const float ratio = static_cast<float>(hp_) / static_cast<float>(maxHp_);

    DrawRectangle(static_cast<int>(barX - 1), static_cast<int>(barY - 1),
                  static_cast<int>(barW + 2), static_cast<int>(barH + 2), {30, 30, 40, 200});
    raylib::Color hpColor = (ratio > 0.5f) ? GREEN : ((ratio > 0.25f) ? ORANGE : RED);
    DrawRectangle(static_cast<int>(barX), static_cast<int>(barY),
                  static_cast<int>(barW * ratio), static_cast<int>(barH), hpColor);

    // HP text
    const std::string hpStr = std::to_string(hp_) + "/" + std::to_string(maxHp_);
    const int fs = 12;
    const int tw = MeasureText(hpStr.c_str(), fs);
    DrawText(hpStr.c_str(), bx + bw / 2 - tw / 2, by - 28, fs, LIGHTGRAY);
}
