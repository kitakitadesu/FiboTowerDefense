#pragma once

#include <memory>
#include <vector>

#include "board.hpp"
#include "id_generator.hpp"
#include "identifier.hpp"
#include "level.hpp"
#include "scoreboard.hpp"
#include "sprite.hpp"
#include "tower.hpp"

enum class GameState {
    Menu,
    Playing,
    Paused,
    Won,
    Lost
};

/// Top-level game state machine. Owns Board, Tower, Scoreboard, and current Level.
class Game : public IIdentifier {
public:
    Game();
    ~Game();

    int getId() const override { return id_; }

    /// Load textures (call once after Window created, before Game loop)
    void init();

    void start();
    bool isRunning() const { return running_; }
    void quit() { running_ = false; }

    GameState getGameState() const { return state_; }

    void update(float dt);
    void render();
    void renderEndScreen();

    bool shouldRestart() const { return shouldRestart_; }
    void resetForRestart();

    Board&      getBoard()        { return board_; }
    Level*      getCurrentLevel() { return currentLevel_.get(); }
    Scoreboard& getScoreboard()   { return scoreboard_; }

private:
    void rebuildWaypoints();
    void handleCheatKey(int key);

    int id_;
    bool running_ = true;
    bool shouldRestart_ = false;
    GameState state_ = GameState::Menu;

    Board board_;
    Tower tower_;
    Scoreboard scoreboard_;
    std::unique_ptr<Level> currentLevel_;

    bool bossDefeated_ = false;
    bool cheatMode_ = false;
    int  cheatIdx_ = 0;
    std::vector<int> cheatSeq_;

    Sprite gooseTex_;
    std::vector<std::vector<raylib::Vector2>> laneWps_;

    Music menuMusic_;
    Music dayMusic_;
    // Music nightMusic_;
    Music* currentMusic_ = nullptr; // ตัวชี้ว่าตอนนี้เล่นเพลงไหนอยู่

    // sfx
    Sound clickSound_;

    void switchMusic(Music* newMusic);
};
