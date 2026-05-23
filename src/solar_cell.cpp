#include "solar_cell.hpp"

#include <string>

SolarCell::SolarCell(int col, int row) : m_col(col), m_row(row) {}

int SolarCell::update(float dt, bool isNight) {
    if (isNight) return 0;
    m_timer += dt;
    const float interval = getInterval();
    if (m_timer >= interval) {
        m_timer -= interval;
        return getReward();
    }
    return 0;
}

void SolarCell::draw(const raylib::Texture* tex, raylib::Vector2 screenPos, float cellW, float cellH) const {
    const float s = std::min(cellW, cellH) * 0.85f;
    const float h = s / 2.0f;

    if (tex) {
        const Rectangle src{0, 0,
                            static_cast<float>(tex->width),
                            static_cast<float>(tex->height)};
        const Rectangle dst{screenPos.x - h, screenPos.y - h, s, s};
        tex->Draw(src, dst, {}, 0.0f, raylib::Color::White());
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
    DrawRectangle(static_cast<int>(badgeX - 4), static_cast<int>(badgeY - 2),
                  lw + 8, 16, {30, 30, 40, 200});
    DrawText(lvlStr.c_str(), static_cast<int>(badgeX), static_cast<int>(badgeY), lf, WHITE);
}
