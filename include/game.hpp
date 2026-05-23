#pragma once

#include <memory>
#include <vector>

#include "game_board.hpp"
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
    Countdown,
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

    GameBoard& getBoard() { return board_; }

private:
    void rebuildWaypoints();
    void handleCheatKey(int key);

    int id_;
    bool running_ = true;
    bool shouldRestart_ = false;
    GameState state_ = GameState::Menu;

    GameBoard board_;
    Tower tower_;
    Scoreboard scoreboard_;
    std::unique_ptr<Level> currentLevel_;

    bool bossDefeated_ = false;
    bool cheatMode_ = false;
    int  cheatIdx_ = 0;
    std::vector<int> cheatSeq_;

    // Entity sprites (day/night variants)
    Sprite ur3eDay_{"assets/ur3e_day_64x64.png"};
    Sprite ur3eNight_{"assets/ur3e_night_64x64.png"};
    Sprite gooseDay_{"assets/goose_day_64x64.png"};
    Sprite gooseNight_{"assets/goose_night_64x64.png"};
    Sprite solarDay_{"assets/solar_day_64x64.png"};
    Sprite solarNight_{"assets/solar_night_64x64.png"};
    Sprite manDay_{"assets/man_day_64x64.png"};
    Sprite manNight_{"assets/man_night_64x64.png"};
    Sprite menuImage_{"assets/GameMenu_edit2.png"};
    std::vector<std::vector<raylib::Vector2>> laneWps_;

    Texture2D nightMapTex_;
    bool isNight_ = false;
    float nightAlpha_ = 0.0f;
    bool boardIsNight_ = false;

    Music menuMusic_;
    Music dayMusic_;
    Music nightMusic_;
    Music* currentMusic_ = nullptr; // ตัวชี้ว่าตอนนี้เล่นเพลงไหนอยู่

    // sfx
    Sound clickSound_;
    Sound countdownBeep_;
    int   lastCountdownNum_ = -1;
    Sound gameOverSound_;
    bool  gameOverSoundPlayed_ = false;
    Sound towerHitSound_;
    

    // Name input on end screen
    char  nameInput_[32] = "";
    bool  scoreSaved_ = false;
    bool  nameEditing_ = false;

    void switchMusic(Music* newMusic);

    float countdownTimer_ = 0.0f;
};
