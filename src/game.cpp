#include "game.hpp"

#include <algorithm>
#include <cmath>
#include <string>

#include <raygui.h>
#include <rlgl.h>  // for rlPushMatrix/rlTranslatef/rlScalef/rlPopMatrix

Game::Game()
    : id_(IdGenerator::getNextId()),
      board_("assets/map_day_0.png"),
      tower_(25),
      cheatSeq_{KEY_B, KEY_O, KEY_C, KEY_C, KEY_H, KEY_I},
      gooseTex_("assets/goose_day_0.png")
{
    laneWps_.resize(board_.getRowCount());
    rebuildWaypoints();
    menuOverlay_.snapTo(0.0f);
    menuOverlay_.setTarget(1.0f);  // menu scales in on startup
    menuTransitioning_ = false;
}

Game::~Game() = default;

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
}

void Game::start() {
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
    scoreboard_ = Scoreboard();  // reset scores
    // Tower will be reset in start()
    // Textures stay loaded (they have the GL context)
    start();
}

void Game::update(float dt) {
    if (!running_) return;

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

    // ── menu spring + transition ──
    if (state_ == GameState::Menu) {
        menuOverlay_.update(dt);
        if (menuTransitioning_ && menuOverlay_.isSettled(0.01f)) {
            start();
        }
    }

    // ── pause toggle ──
    if ((state_ == GameState::Playing || state_ == GameState::Paused) &&
        (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE))) {
        state_ = (state_ == GameState::Playing) ? GameState::Paused : GameState::Playing;
    }

    // ── pause overlay spring (scale from centroid) ──
    pauseOverlay_.setTarget(state_ == GameState::Paused ? 1.0f : 0.0f);
    pauseOverlay_.update(dt);

    if (state_ == GameState::Playing && currentLevel_) {
        currentLevel_->update(dt, laneWps_);

        if (currentLevel_->isTowerDestroyed()) {
            state_ = GameState::Lost;
            scoreboard_.addScore("Player", scoreboard_.getCurrentScore());
        } else if (currentLevel_->getWaveManager().allWavesDone() &&
                   currentLevel_->getEnemies().empty()) {
            state_ = GameState::Won;
            scoreboard_.addScore("Player", scoreboard_.getCurrentScore());
        }
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    const raylib::Texture* gooseRaw = &gooseTex_.getTexture();

    if (currentLevel_) {
        currentLevel_->render(gooseRaw);
        currentLevel_->renderUI();
    }

    // ── Cheat indicator ──
    if (cheatMode_) {
        raylib::DrawText("CHEAT ON", GetScreenWidth() - 120, GetScreenHeight() - 40, 20, GREEN);
    }

    // ── Menu (spring scale from centroid) ──
    if (state_ == GameState::Menu) {
        // Board behind the overlay (always full size)
        board_.draw();

        const int w = GetScreenWidth(), h = GetScreenHeight();
        const int cx = w / 2, cy = h / 2;

        // Dim background (constant, full screen)
        DrawRectangle(0, 0, w, h, {0, 0, 0, 150});

        const float s = std::clamp(menuOverlay_.getValue(), 0.0f, 1.2f);
        if (s > 0.001f) {
            // ── Panel + content inside centroid-scale transform ──
            rlPushMatrix();
            rlTranslatef(static_cast<float>(cx), static_cast<float>(cy), 0.0f);
            rlScalef(s, s, 1.0f);
            rlTranslatef(static_cast<float>(-cx), static_cast<float>(-cy), 0.0f);

            DrawRectangleRounded({static_cast<float>(cx - 230),
                                  static_cast<float>(cy - 185),
                                  460.0f, 370.0f},
                                 0.2f, 10, {20, 20, 20, 230});

            // Title with shadow
            const char* title = "FIBO TOWER DEFENSE";
            raylib::DrawText(title, cx - MeasureText(title, 42) / 2 + 3, cy - 150, 42, BLACK);
            raylib::DrawText(title, cx - MeasureText(title, 42) / 2, cy - 153, 42, SKYBLUE);

            // Subtitle
            const char* sub = "Defend your tower.  Build.  Upgrade.  Survive.";
            raylib::DrawText(sub, cx - MeasureText(sub, 14) / 2, cy - 100, 14, {200, 200, 200, 200});

            // PLAY button (inside transform, manual click/key outside)
            const float bL = static_cast<float>(cx - 90);
            const float bT = static_cast<float>(cy - 55);
            const float bW = 180.0f, bH = 50.0f;

            DrawRectangleRounded({bL, bT, bW, bH}, 0.2f, 8,
                Color{60, 100, 180, 230});
            const char* btnText = "PLAY";
            raylib::DrawText(btnText,
                static_cast<int>(bL + bW / 2 - MeasureText(btnText, 22) / 2),
                static_cast<int>(bT + bH / 2 - 11),
                22, WHITE);

            // Hint + controls text
            raylib::DrawText("ENTER / SPACE to start",
                cx - MeasureText("ENTER / SPACE to start", 14) / 2, cy + 15,
                14, LIGHTGRAY);

            const int cfs = 13;
            const int cY = cy + 45;
            const char* ctrl1 = "[T] Shooting Turret  [M] Melee Turret  [S] Solar Cell";
            const char* ctrl2 = "[P/Space/ESC] Pause  [RClick] Cancel build  [Click] Select";
            const char* ctrl3 = "Block enemies by placing turrets in their lane";
            raylib::DrawText(ctrl1, cx - MeasureText(ctrl1, cfs) / 2, cY, cfs, {180, 180, 180, 200});
            raylib::DrawText(ctrl2, cx - MeasureText(ctrl2, cfs) / 2, cY + 18, cfs, {180, 180, 180, 200});
            raylib::DrawText(ctrl3, cx - MeasureText(ctrl3, cfs) / 2, cY + 36, cfs, {140, 140, 140, 160});

            // Credits
            raylib::DrawText("raylib / raygui",
                cx - MeasureText("raylib / raygui", 11) / 2, cy + 120,
                11, {80, 80, 80, 180});

            rlPopMatrix();

            // ── Hit / key detection (outside transform, gated by !menuTransitioning_) ──
            const raylib::Vector2 mp = GetMousePosition();
            const float scrX = (bL - cx) * s + cx;
            const float scrY = (bT - cy) * s + cy;
            const float scrW = bW * s, scrH = bH * s;
            const bool hitPlay = mp.x >= scrX && mp.x < scrX + scrW
                              && mp.y >= scrY && mp.y < scrY + scrH;

            if (!menuTransitioning_) {
                if ((hitPlay && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    menuTransitioning_ = true;
                    menuOverlay_.setTarget(0.0f);
                }
            }
        }
    }

    // ── Pause button (always visible when playing/paused) ──
    if (state_ == GameState::Playing || state_ == GameState::Paused) {
        const int btnS = 36;
        const int btnX = GetScreenWidth() - btnS - 15;
        const int btnY = 15;
        const char* icon = (state_ == GameState::Paused) ? "\xE2\x96\xB6" : "\xE2\x96\xB6\xE2\x96\xB6";

        DrawRectangleRounded({static_cast<float>(btnX), static_cast<float>(btnY),
                              static_cast<float>(btnS), static_cast<float>(btnS)},
                             0.2f, 8, {0, 0, 0, 180});
        raylib::DrawText(icon, btnX + 8, btnY + 6, 20, WHITE);

        if (GuiButton({static_cast<float>(btnX), static_cast<float>(btnY),
                       static_cast<float>(btnS), static_cast<float>(btnS)}, "")) {
            state_ = (state_ == GameState::Paused) ? GameState::Playing : GameState::Paused;
        }
    }

    // ── Pause overlay (spring scale from centroid) ──
    {
        const float s = std::clamp(pauseOverlay_.getValue(), 0.0f, 1.2f);
        if (s > 0.001f) {
            const int w = GetScreenWidth(), h = GetScreenHeight();
            const int cx = w / 2, cy = h / 2;

            // Dim background (constant, full screen)
            DrawRectangle(0, 0, w, h, {0, 0, 0, 150});

            // ── Panel + content inside centroid-scale transform ──
            rlPushMatrix();
            rlTranslatef(static_cast<float>(cx), static_cast<float>(cy), 0.0f);
            rlScalef(s, s, 1.0f);
            rlTranslatef(static_cast<float>(-cx), static_cast<float>(-cy), 0.0f);

            DrawRectangleRounded({static_cast<float>(cx - 150),
                                  static_cast<float>(cy - 90),
                                  300.0f, 180.0f},
                                 0.2f, 10, {20, 20, 20, 230});

            const char* pauseMsg = "PAUSED";
            raylib::DrawText(pauseMsg,
                cx - MeasureText(pauseMsg, 50) / 2 + 3, cy - 70,
                50, BLACK);
            raylib::DrawText(pauseMsg,
                cx - MeasureText(pauseMsg, 50) / 2, cy - 73,
                50, WHITE);

            // RESUME button (inside transform, manual click outside)
            const float bL = static_cast<float>(cx - 80);
            const float bT = static_cast<float>(cy - 5);
            const float bW = 160.0f, bH = 45.0f;

            // Screen-space button bounds for hi-test
            const float scrX = (bL - cx) * s + cx;
            const float scrY = (bT - cy) * s + cy;
            const float scrW = bW * s, scrH = bH * s;

            const raylib::Vector2 mp = GetMousePosition();
            const bool hover = mp.x >= scrX && mp.x < scrX + scrW
                            && mp.y >= scrY && mp.y < scrY + scrH;

            DrawRectangleRounded({bL, bT, bW, bH}, 0.2f, 8,
                hover ? Color{60, 60, 60, 230} : Color{40, 40, 40, 230});

            const char* btnText = "RESUME";
            raylib::DrawText(btnText,
                static_cast<int>(bL + bW / 2 - MeasureText(btnText, 20) / 2),
                static_cast<int>(bT + bH / 2 - 10),
                20, WHITE);

            rlPopMatrix();

            // Click detection (screen-space, outside transform)
            if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                state_ = GameState::Playing;

            // Bottom hint (outside transform, always normal size)
            raylib::DrawText("P / SPACE to resume",
                cx - MeasureText("P / SPACE to resume", 14) / 2, cy + 60,
                14, {150, 150, 150, 200});
        }
    }

    EndDrawing();
}

void Game::renderEndScreen() {
    BeginDrawing();
    ClearBackground({20, 20, 20, 255});

    const int cx = GetScreenWidth() / 2, cy = GetScreenHeight() / 2;
    DrawRectangleRounded({static_cast<float>(cx - 210), static_cast<float>(cy - 150),
                          420.0f, 300.0f}, 0.2f, 10, {40, 40, 40, 255});

    const bool isGameOver = (state_ == GameState::Lost);
    const char* msg = isGameOver ? "GAME OVER" : "VICTORY!";
    raylib::DrawText(msg, cx - MeasureText(msg, 50) / 2 + 3, cy - 115, 50, BLACK);
    raylib::DrawText(msg, cx - MeasureText(msg, 50) / 2, cy - 118, 50, isGameOver ? RED : GREEN);

    std::string scoreStr = "Score: " + std::to_string(scoreboard_.getCurrentScore());
    raylib::DrawText(scoreStr.c_str(), cx - MeasureText(scoreStr.c_str(), 26) / 2, cy - 45, 26, GOLD);

    if (GuiButton({static_cast<float>(cx - 90), static_cast<float>(cy + 30), 180.0f, 45.0f}, "PLAY AGAIN")
        || IsKeyPressed(KEY_R)) {
        shouldRestart_ = true;
    }

    if (GuiButton({static_cast<float>(cx - 90), static_cast<float>(cy + 85), 180.0f, 45.0f}, "QUIT")
        || IsKeyPressed(KEY_ESCAPE)) {
        running_ = false;
    }

    raylib::DrawText("R = Play Again  |  ESC = Quit",
        cx - MeasureText("R = Play Again  |  ESC = Quit", 12) / 2, cy + 145, 12, {100, 100, 100, 180});

    EndDrawing();
}
