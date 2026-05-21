#pragma once

#include <vector>

#include "raylib-cpp.hpp"

class Board;

/// Build lane waypoints (screen coords) from right→left through grid cells.
std::vector<raylib::Vector2> buildLaneWaypoints(const Board& board, int row);

/// Lane-based enemy that walks from right to left along a row (PvZ-style).
class Enemy {
public:
    enum State { WALKING, DYING, DEAD, ESCAPED };

    /// @param row       Grid row (lane) this enemy walks in
    /// @param hp        Hit points
    /// @param speed     Movement speed (pixels/sec)
    /// @param reward    Gold given on kill
    Enemy(int row, int hp, float speed, int reward);

    void update(float dt, const std::vector<raylib::Vector2>& waypoints);

    /// Draw at current position using the shared texture.
    void draw(const raylib::Texture* tex) const;

    void takeDamage(int dmg);

    // -- getters --
    int     getRow()    const { return row_; }
    float   getHp()     const { return hp_; }
    float   getMaxHp()  const { return maxHp_; }
    State   getState()  const { return state_; }
    int     getReward() const { return reward_; }
    raylib::Vector2 getPosition() const { return pos_; }
    bool    isAlive()   const { return state_ == WALKING; }
    bool    isDead()    const { return state_ == DEAD; }
    bool    isEscaped() const { return state_ == ESCAPED; }

    float getRadius() const { return 20.0f; }  ///< collision radius

private:
    int   row_;
    int   waypointIdx_ = 0;
    float hp_;
    float maxHp_;
    float speed_;
    int   reward_;
    State state_ = WALKING;
    raylib::Vector2 pos_;
    raylib::Vector2 vel_;
};
