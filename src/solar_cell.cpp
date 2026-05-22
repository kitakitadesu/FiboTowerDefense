#include "solar_cell.hpp"

#include <string>

SolarCell::SolarCell(int col, int row) : m_col(col), m_row(row) {}

int SolarCell::update(float dt) {
    m_timer += dt;
    const float interval = getInterval();
    if (m_timer >= interval) {
        m_timer -= interval;
        return getReward();
    }
    return 0;
}

void SolarCell::draw(const raylib::Texture* tex, raylib::Vector2 screenPos) const {
    const float s = 50.0f;
    const float h = s / 2.0f;

    if (tex) {
        const Rectangle src{0, 0,
                            static_cast<float>(tex->width),
                            static_cast<float>(tex->height)};
        const Rectangle dst{screenPos.x - h, screenPos.y - h, s, s};
        tex->Draw(src, dst, {}, 0.0f, raylib::Color{100, 255, 100, 255});
    } else {
        DrawRectangle(static_cast<int>(screenPos.x - h),
                      static_cast<int>(screenPos.y - h),
                      static_cast<int>(s), static_cast<int>(s), GREEN);
    }

    // Level badge
    const std::string lvlStr = std::to_string(m_level);
    const int lf = 12;
    const int lw = MeasureText(lvlStr.c_str(), lf);
    const float badgeX = screenPos.x - lw / 2.0f;
    const float badgeY = screenPos.y - h - 8;
    DrawRectangleRounded({badgeX - 4, badgeY - 2, static_cast<float>(lw + 8), 16.0f},
                         0.4f, 6, {0, 0, 0, 180});
    DrawText(lvlStr.c_str(), static_cast<int>(badgeX), static_cast<int>(badgeY), lf, WHITE);
}
