#include "game.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>

#include <raygui.h>

Game::Game()
    : id_(IdGenerator::getNextId()),
      board_(),
      tower_(25),
      cheatSeq_{KEY_B, KEY_O, KEY_C, KEY_C, KEY_H, KEY_I},
      gooseTex_("assets/goose_day_0.png")
{
    laneWps_.resize(board_.getRowCount());
    rebuildWaypoints();
}

Game::~Game() {
    UnloadMusicStream(menuMusic_);
    UnloadMusicStream(dayMusic_);
    UnloadMusicStream(nightMusic_);

    UnloadTexture(nightMapTex_);

    UnloadSound(countdownBeep_);
    UnloadSound(clickSound_);
}

void Game::switchMusic(Music* newMusic) {
    if (currentMusic_ != newMusic) {
        if (currentMusic_ != nullptr) {
            StopMusicStream(*currentMusic_);
        }
        currentMusic_ = newMusic;
        PlayMusicStream(*currentMusic_);
    }
}

void Game::rebuildWaypoints() {
    for (int r = 0; r < board_.getRowCount(); ++r)
        laneWps_[r] = buildLaneWaypoints(board_, r);
}

void Game::handleCheatKey(int key) {
    if (cheatMode_) return;
    if (key == cheatSeq_[cheatIdx_]) {
        ++cheatIdx_;
        if (cheatIdx_ >= static_cast<int>(cheatSeq_.size())) {
            cheatMode_ = true;
            cheatIdx_ = 0;
        }
        return;
    }
    cheatIdx_ = (key == cheatSeq_.front()) ? 1 : 0;
}

void Game::init() {
    board_.loadTexture();
    gooseTex_.loadTexture();

    nightMapTex_ = LoadTexture("assets/map1_night.png");

    menuImage_.loadTexture();
    menuMusic_ = LoadMusicStream("assets/dayMusic_Signal_at_the_Tower.mp3");
    dayMusic_ = LoadMusicStream("assets/menuMusic_Clocktower_Circuit.mp3");
    nightMusic_ = LoadMusicStream("assets/nightMusic_Midnight_Campus_Siege.mp3");

    countdownBeep_ = LoadSound("assets/error_007.ogg");

    clickSound_ = LoadSound("assets/toggle_002.ogg");

    scoreboard_.load("scores.dat");

    switchMusic(&menuMusic_);
}

void Game::start() {
    isNight_ = false;
    nightAlpha_ = 0.0f;
    // ถ้าอนาคตมีด่านกลางคืน ค่อยเขียน if เช็คตรงนี้
    switchMusic(&dayMusic_);

    board_.updateScale(GetScreenWidth());
    rebuildWaypoints();

    // ── fresh Tower (reset HP) ──
    tower_ = Tower(25);

    // ── create Level with 5 default waves ──
    auto level = std::make_unique<Level>(board_, tower_, scoreboard_);
    auto& wm = level->getWaveManager();
    auto addW = [&](int cnt, float interv, int hp, float spd, int rew) {
        WaveDef w;
        w.enemyCount = cnt; w.spawnInterval = interv; w.hp = hp; w.speed = spd; w.reward = rew;
        wm.addWave(w);
    };
    addW(5, 1.5f, 70, 50.0f, 32);
    addW(8, 1.2f, 85, 55.0f, 37);
    addW(12, 1.0f, 100, 60.0f, 42);
    addW(15, 0.9f, 130, 65.0f, 47);
    addW(20, 0.8f, 170, 70.0f, 52);
    level->start();
    currentLevel_ = std::move(level);

    state_ = GameState::Playing;
}

void Game::resetForRestart() {
    shouldRestart_ = false;
    cheatMode_ = false;
    cheatIdx_ = 0;
    bossDefeated_ = false;
    scoreboard_ = Scoreboard();
    nameInput_[0] = '\0';
    scoreSaved_ = false;
    nameEditing_ = false;
    // Tower will be reset in start()
    // Textures stay loaded (they have the GL context)
    start();
}

void Game::update(float dt) {
    if (!running_) return;

    // ── update music stream only when playing or on menu ──
    if (currentMusic_ != nullptr && (state_ == GameState::Playing || state_ == GameState::Menu)) {
        UpdateMusicStream(*currentMusic_);
    }

    // ── aspect-ratio lock ──
    if (IsWindowResized()) {
        const int curW = GetScreenWidth();
        const int curH = GetScreenHeight();
        const float curAspect = static_cast<float>(curW) / static_cast<float>(curH);
        if (std::abs(curAspect - board_.getAspectRatio()) > 0.001f) {
            const int clampedH = static_cast<int>(std::round(static_cast<float>(curW) / board_.getAspectRatio()));
            SetWindowSize(curW, clampedH);
        }
        board_.updateScale(GetScreenWidth());
        rebuildWaypoints();
    }

    // ── cheat input ──
    {
        int key = GetKeyPressed();
        while (key > 0) { handleCheatKey(key); key = GetKeyPressed(); }
        if (currentLevel_) currentLevel_->setCheatMode(cheatMode_);
    }

    // ── pause toggle ──
    if ((state_ == GameState::Playing || state_ == GameState::Paused) &&
        (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE))) {
        if (state_ == GameState::Playing) {
            state_ = GameState::Paused;
            if (currentMusic_) PauseMusicStream(*currentMusic_);
        } else {
            state_ = GameState::Countdown;
            countdownTimer_ = 3.0f;
            // if (currentMusic_) ResumeMusicStream(*currentMusic_);
        }
    }

    // --- cooldown ---
    if (state_ == GameState::Countdown) {
        if (currentLevel_) {
            currentLevel_->update(0.0f, laneWps_); 
        }

        countdownTimer_ -= dt; 

        int currentNum = static_cast<int>(ceil(countdownTimer_));
        if (currentNum != lastCountdownNum_ && currentNum >= 1) {
            SetSoundPitch(countdownBeep_, 1.0f + (3 - currentNum) * 0.15f); // 1, 2, 3 เสียงสูงขึ้นเรื่อยๆ
            PlaySound(countdownBeep_);
            lastCountdownNum_ = currentNum;
        }

        if (countdownTimer_ <= 0.0f) {
            state_ = GameState::Playing; 
            SetSoundPitch(countdownBeep_, 1.5f); 
            PlaySound(countdownBeep_);
            lastCountdownNum_ = -1;  // reset
            if (currentMusic_) ResumeMusicStream(*currentMusic_);
        }
    }

    if (state_ == GameState::Playing && currentLevel_) {
        currentLevel_->update(dt, laneWps_);

        int currentWave = currentLevel_->getWaveManager().getCurrentWave();
        bool shouldBeNight = (currentWave / 10) % 2 == 1; 

        if (shouldBeNight != isNight_) {
            isNight_ = shouldBeNight;
            if (isNight_) switchMusic(&nightMusic_);
            else switchMusic(&dayMusic_);
        }

        float fadeSpeed = 100.0f; 
        float targetAlpha = isNight_ ? 255.0f : 0.0f;
        
        if (nightAlpha_ < targetAlpha) {
            nightAlpha_ += fadeSpeed * dt;
            if (nightAlpha_ > targetAlpha) nightAlpha_ = targetAlpha;
        } else if (nightAlpha_ > targetAlpha) {
            nightAlpha_ -= fadeSpeed * dt;
            if (nightAlpha_ < targetAlpha) nightAlpha_ = targetAlpha;
        }

        // Toggle board layers when alpha crosses 128 midpoint
        if (!boardIsNight_ && nightAlpha_ >= 128.0f) {
            board_.setNightMode(true);
            boardIsNight_ = true;
        } else if (boardIsNight_ && nightAlpha_ <= 128.0f && !isNight_) {
            board_.setNightMode(false);
            boardIsNight_ = false;
        }

        if (currentLevel_->isTowerDestroyed()) {
            state_ = GameState::Lost;
        } else if (currentLevel_->getWaveManager().allWavesDone() &&
                   currentLevel_->getEnemies().empty()) {
            state_ = GameState::Won;
        }
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    const raylib::Texture* gooseRaw = &gooseTex_.getTexture();

    if (currentLevel_) {
        currentLevel_->render(gooseRaw, boardIsNight_ ? nullptr : &nightMapTex_, nightAlpha_);
        currentLevel_->setPaused(state_ == GameState::Paused);
        currentLevel_->renderUI();
    }

    // ── Cheat indicator (hidden on menu) ──
    if (cheatMode_ && state_ != GameState::Menu) {
        raylib::DrawText("CHEAT ON", GetScreenWidth() - 120, GetScreenHeight() - 40, 20, Color{255, 140, 20, 255});
    }

    // ── Menu ──
    if (state_ == GameState::Menu) {
        const auto& menuTex = menuImage_.getTexture();

        const float screenW = static_cast<float>(GetScreenWidth());
        const float screenH = static_cast<float>(GetScreenHeight());
        const Rectangle sourceRec = {0.0f, 0.0f, static_cast<float>(menuTex.width), static_cast<float>(menuTex.height)};
        const Rectangle destRec = {0.0f, 0.0f, screenW, screenH};
        DrawTexturePro(menuTex, sourceRec, destRec, {0.0f, 0.0f}, 0.0f, WHITE);

        // Start button (image-based hot zone)
        const Rectangle startBtnBounds = {screenW / 2 - 130, screenH / 2 + 28, 253, 88};
        const Rectangle quitBtnBounds  = {screenW / 2 - 130, screenH / 2 + 139, 253, 78};
        const Vector2 mousePos = GetMousePosition();
        const bool hoverStart = CheckCollisionPointRec(mousePos, startBtnBounds);
        const bool hoverQuit  = CheckCollisionPointRec(mousePos, quitBtnBounds);
        const bool pressEnter = IsKeyDown(KEY_ENTER) || IsKeyDown(KEY_SPACE);

        float time = (float)GetTime();
        unsigned char textAlpha = (unsigned char)((cos(time * 4.0f) + 1.0f) * 127.5f);
        DrawText("WAITING FOR PLAYER...", 20, screenH - 40, 20, { 255, 255, 255, textAlpha });

        // Start button visual feedback
        if (hoverStart || pressEnter) {
            const bool pressing = (pressEnter || IsMouseButtonDown(MOUSE_LEFT_BUTTON));
            DrawRectangleRec(startBtnBounds, pressing ? Color{0, 0, 0, 80} : Color{209, 209, 209, 80});
        }
        // Quit button visual feedback
        if (hoverQuit) {
            const bool pressing = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
            DrawRectangleRec(quitBtnBounds, pressing ? Color{0, 0, 0, 80} : Color{209, 209, 209, 80});
        }

        // Triggers on release
        if ((hoverStart && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) ||
            IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE))
        {
            PlaySound(clickSound_);
            start();
        }
        if (hoverQuit && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(clickSound_);
            while (IsSoundPlaying(clickSound_)) {
                WaitTime(0.01f); 
            }
            running_ = false;
        }
    }

    // ── Pause button (always visible when playing/paused) ──
    if (state_ == GameState::Playing || state_ == GameState::Paused) {
        const int btnS = 36;
        const int btnX = GetScreenWidth() - btnS - 15;
        const int btnY = 15;
        const char* icon = (state_ == GameState::Paused) ? "\xE2\x96\xB6" : "II";

        DrawRectangle(btnX, btnY, btnS, btnS, {15, 15, 25, 200});
        raylib::DrawText(icon, btnX + 8, btnY + 6, 20, WHITE);

        if (GuiButton({static_cast<float>(btnX), static_cast<float>(btnY),
                       static_cast<float>(btnS), static_cast<float>(btnS)}, "")) {
            if (state_ == GameState::Paused) {
                state_ = GameState::Playing;
                if (currentMusic_) ResumeMusicStream(*currentMusic_);
            } else {
                state_ = GameState::Paused;
                if (currentMusic_) PauseMusicStream(*currentMusic_);
            }
        }
    }

    // ── Pause overlay ──
    if (state_ == GameState::Paused) {
        const int w = GetScreenWidth(), h = GetScreenHeight();
        const int cx = w / 2, cy = h / 2;
        DrawRectangle(0, 0, w, h, {0, 0, 0, 150});
        DrawRectangle(cx - 150, cy - 90, 300, 180, {15, 15, 25, 240});

        const char* pauseMsg = "PAUSED";
        raylib::DrawText(pauseMsg, cx - MeasureText(pauseMsg, 50) / 2 + 3, cy - 70, 50, BLACK);
        raylib::DrawText(pauseMsg, cx - MeasureText(pauseMsg, 50) / 2, cy - 73, 50, WHITE);

        if (GuiButton({static_cast<float>(cx - 80), static_cast<float>(cy - 5), 160.0f, 45.0f}, "RESUME")) {
            state_ = GameState::Countdown;
            countdownTimer_ = 3.0f;
            // if (currentMusic_) ResumeMusicStream(*currentMusic_);
        }
        raylib::DrawText("P / SPACE to resume", cx - MeasureText("P / SPACE to resume", 14) / 2, cy + 60, 14, {150, 150, 150, 200});
    }
    if (state_ == GameState::Countdown) {
        const int w = GetScreenWidth(), h = GetScreenHeight();
        
        int displayNum = static_cast<int>(ceil(countdownTimer_)); 
        std::string numStr = std::to_string(displayNum);
        
        float fraction = countdownTimer_ - (displayNum - 1);
        int fontSize = 80 + static_cast<int>(fraction * 40); 

        int textW = MeasureText(numStr.c_str(), fontSize);
        int tx = w / 2 - textW / 2;
        int ty = h / 2 - fontSize / 2;

        DrawText(numStr.c_str(), tx + 4, ty + 4, fontSize, BLACK);

        DrawText(numStr.c_str(), tx, ty, fontSize, Color{255, 140, 20, 255});
    }
    EndDrawing();
}

void Game::renderEndScreen() {
    BeginDrawing();
    ClearBackground({20, 20, 20, 255});

    const int cx = GetScreenWidth() / 2, cy = GetScreenHeight() / 2;
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,   (int)ColorToInt(Color{50, 50, 60, 255}));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,   (int)ColorToInt(Color{200, 200, 210, 255}));
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, (int)ColorToInt(Color{80, 80, 90, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,  (int)ColorToInt(Color{70, 65, 55, 255}));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED,  (int)ColorToInt(Color{255, 255, 255, 255}));
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED,(int)ColorToInt(Color{255, 140, 20, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,  (int)ColorToInt(Color{80, 70, 55, 255}));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED,  (int)ColorToInt(Color{255, 255, 220, 255}));
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED,(int)ColorToInt(Color{255, 160, 40, 255}));

    const bool isGameOver = (state_ == GameState::Lost);
    const char* msg = isGameOver ? "GAME OVER" : "VICTORY!";

    if (!scoreSaved_) {
        // ── Taller panel with name input ──
        DrawRectangle(cx - 210, cy - 175, 420, 410, {25, 25, 35, 255});

        // Title
        raylib::DrawText(msg, cx - MeasureText(msg, 50) / 2 + 3, cy - 135, 50, BLACK);
        raylib::DrawText(msg, cx - MeasureText(msg, 50) / 2, cy - 138, 50, isGameOver ? RED : GREEN);

        // Score
        std::string scoreStr = "Score: " + std::to_string(scoreboard_.getCurrentScore());
        raylib::DrawText(scoreStr.c_str(), cx - MeasureText(scoreStr.c_str(), 26) / 2, cy - 60, 26, GOLD);

        // ── Name input ──
        raylib::DrawText("YOUR NAME:", cx - 160, cy - 15, 16, LIGHTGRAY);
        Rectangle nameRect = {static_cast<float>(cx - 160), static_cast<float>(cy + 12), 220, 30};
        if (GuiTextBox(nameRect, nameInput_, 31, nameEditing_) == 1)
            nameEditing_ = !nameEditing_;

        Rectangle saveRect = {static_cast<float>(cx + 70), static_cast<float>(cy + 12), 90, 30};
        if (GuiButton(saveRect, "SAVE")) {
            if (nameEditing_) nameEditing_ = false;
            if (nameInput_[0] == '\0') strcpy(nameInput_, "Player");
            scoreboard_.addScore(nameInput_, scoreboard_.getCurrentScore());
            scoreboard_.save("scores.dat");
            scoreSaved_ = true;
        }
    } else {
        // ── Normal panel with high scores + actions ──
        DrawRectangle(cx - 210, cy - 150, 420, 410, {25, 25, 35, 255});

        // Title
        raylib::DrawText(msg, cx - MeasureText(msg, 50) / 2 + 3, cy - 115, 50, BLACK);
        raylib::DrawText(msg, cx - MeasureText(msg, 50) / 2, cy - 118, 50, isGameOver ? RED : GREEN);

        // Score
        std::string scoreStr = "Score: " + std::to_string(scoreboard_.getCurrentScore());
        raylib::DrawText(scoreStr.c_str(), cx - MeasureText(scoreStr.c_str(), 26) / 2, cy - 45, 26, GOLD);

        // ── High scores (top 5 to avoid clipping into buttons) ──
        {
            const auto top = scoreboard_.getTopScores(5);
            int hsY = cy + 5;
            raylib::DrawText("HIGH SCORES", cx - MeasureText("HIGH SCORES", 16) / 2, hsY, 16, GRAY);
            hsY += 22;
            for (int i = 0; i < static_cast<int>(top.size()); ++i) {
                std::string line = std::to_string(i + 1) + ". " + top[i].name + " - " + std::to_string(top[i].score);
                bool isCurrent = (i == 0 && top[i].name == nameInput_ && top[i].score == scoreboard_.getCurrentScore());
                raylib::DrawText(line.c_str(), cx - MeasureText(line.c_str(), 14) / 2, hsY,
                                 14, isCurrent ? Color{255, 140, 20, 255} : LIGHTGRAY);
                hsY += 18;
            }
        }

        if (GuiButton({static_cast<float>(cx - 90), static_cast<float>(cy + 130), 180.0f, 45.0f}, "PLAY AGAIN")
            || IsKeyPressed(KEY_R)) {
            shouldRestart_ = true;
        }

        if (GuiButton({static_cast<float>(cx - 90), static_cast<float>(cy + 185), 180.0f, 45.0f}, "QUIT")
            || IsKeyPressed(KEY_ESCAPE)) {
            running_ = false;
        }

        raylib::DrawText("R = Play Again  |  ESC = Quit",
            cx - MeasureText("R = Play Again  |  ESC = Quit", 12) / 2, cy + 245, 12, {100, 100, 100, 180});
    }

    EndDrawing();
}
