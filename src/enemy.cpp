#include "enemy.hpp"

#include <algorithm>
#include <cmath>

#include "game_board.hpp"

// ------------------------------------------------------------------
// Static helper: build lane waypoints from a Board row
// ------------------------------------------------------------------
std::vector<raylib::Vector2> buildLaneWaypoints(const GameBoard& board, int row) {
    std::vector<raylib::Vector2> wps;

    const float margin = 250.0f;
    const int   cols   = board.getColCount();
    const float rowCenterY = GameBoard::kOriginY
        + static_cast<float>(row) * GameBoard::kCellH + GameBoard::kCellH / 2.0f;

    // Image coordinates — stable across window resizes
    const float rightEdge = GameBoard::kOriginX + static_cast<float>(cols) * GameBoard::kCellW;
    wps.emplace_back(rightEdge + margin, rowCenterY);                // spawn off right
    for (int c = cols - 1; c >= 0; --c)
        wps.emplace_back(GameBoard::kOriginX + static_cast<float>(c) * GameBoard::kCellW + GameBoard::kCellW / 2.0f, rowCenterY);  // cell centers
    wps.emplace_back(GameBoard::kOriginX, rowCenterY);              // exit at building (left edge of grid)

    return wps;
}

// ------------------------------------------------------------------
// Enemy implementation
// ------------------------------------------------------------------
Enemy::Enemy(int row, int hp, float speed, int reward)
    : id_(IdGenerator::getNextId()), row_(row), hp_(static_cast<float>(hp)),
      maxHp_(static_cast<float>(hp)), speed_(speed), reward_(reward)
{}

void Enemy::update(float dt, const std::vector<raylib::Vector2>& waypoints) {
    if (state_ != WALKING) return;

    if (waypointIdx_ >= static_cast<int>(waypoints.size())) {
        state_ = ESCAPED;
        return;
    }

    const raylib::Vector2 target = waypoints[waypointIdx_];
    const raylib::Vector2 diff   = target - pos_;

    const float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist < 2.0f) {
        ++waypointIdx_;
        vel_ = raylib::Vector2{};
        return;
    }

    const float maxStep = speed_ * dt;
    if (dist <= maxStep) {
        pos_ = target;
        ++waypointIdx_;
        vel_ = raylib::Vector2{};
    } else {
        vel_ = diff / dist * maxStep;
        pos_ = pos_ + vel_;
    }
}

void Enemy::draw(const raylib::Texture* tex, float cellW, float cellH) const {
    if (state_ != WALKING) return;

    const float s = std::min(cellW, cellH) * 0.85f;
    const float h = s / 2;

    if (tex) {
        const Rectangle src{0, 0, static_cast<float>(tex->width), static_cast<float>(tex->height)};
        const Rectangle dst{pos_.x - h, pos_.y - h, s, s};
        tex->Draw(src, dst, {}, 0.0f, raylib::Color::White());
    } else {
        // Fallback: orange circle
        DrawCircle(static_cast<int>(pos_.x), static_cast<int>(pos_.y),
                   static_cast<int>(h), ORANGE);
    }

    // HP bar above
    const float barW = 40.0f;
    const float barH = 6.0f;
    const float barX = pos_.x - barW / 2;
    const float barY = pos_.y - h - 12;
    const float ratio = std::max(0.0f, hp_ / maxHp_);
    
    // Border/Background
    DrawRectangle(static_cast<int>(barX - 1), static_cast<int>(barY - 1),
                  static_cast<int>(barW + 2), static_cast<int>(barH + 2), {30, 30, 40, 200});
    // HP fill
    raylib::Color hpColor = (ratio > 0.5f) ? GREEN : ((ratio > 0.25f) ? ORANGE : RED);
    DrawRectangle(static_cast<int>(barX), static_cast<int>(barY),
                  static_cast<int>(barW * ratio), static_cast<int>(barH), hpColor);
}

void Enemy::takeDamage(int dmg) {
    if (state_ != WALKING) return;
    hp_ -= static_cast<float>(dmg);
    if (hp_ <= 0.0f) {
        hp_ = 0.0f;
        state_ = DEAD;
    }
}
