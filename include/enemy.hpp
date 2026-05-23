#pragma once

#include <string>
#include <vector>

#include "raylib-cpp.hpp"

#include "id_generator.hpp"
#include "identifier.hpp"
#include "idisplay_name.hpp"
#include "isprite.hpp"

class GameBoard;

/// Build lane waypoints (screen coords) from right→left.
std::vector<raylib::Vector2> buildLaneWaypoints(const GameBoard& board, int row);

/// Lane-based enemy that walks from right to left along a row (PvZ-style).
class Enemy : public IIdentifier, public IDisplayName, public ISprite {
public:
    enum State { WALKING, DYING, DEAD, ESCAPED };

    Enemy(int row, int hp, float speed, int reward);

    int getId() const override { return id_; }
    std::string getDisplayName() const override { return "Enemy"; }

    // ISprite
    void draw() const override {}
    void draw(const raylib::Texture* tex, float cellW, float cellH) const;

    void update(float dt, const std::vector<raylib::Vector2>& waypoints);
    void takeDamage(int dmg);
    void setEscaped() { if (state_ == WALKING) state_ = ESCAPED; }

    void spawnAt(const raylib::Vector2& pos) {
        pos_ = pos;
        waypointIdx_ = 1;
    }

    // -- getters --
    int getWaypointIdx() const { return waypointIdx_; }
    void setPosition(raylib::Vector2 p) { pos_ = p; }

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
    int   id_;
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
