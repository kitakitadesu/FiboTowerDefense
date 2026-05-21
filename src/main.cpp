#include "board.hpp"
#include "enemy.hpp"
#include "projectile.hpp"
#include "sprite.hpp"
#include "tower.hpp"
#include "wave_manager.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ── helpers ────────────────────────────────────────────────────────────

/// Erase elements matching a predicate, return count removed.
template <typename T, typename Pred>
int eraseIf(std::vector<T>& vec, Pred pred) {
    auto it = std::remove_if(vec.begin(), vec.end(), pred);
    int n = static_cast<int>(vec.end() - it);
    vec.erase(it, vec.end());
    return n;
}

// ── main ───────────────────────────────────────────────────────────────

int main() {
    Board board("assets/map_day_0.png");

    const int kInitW = static_cast<int>(720.0f * board.getAspectRatio());
    const int kInitH = 720;

    raylib::Window window(kInitW, kInitH, "FiboTowerDefense", FLAG_WINDOW_RESIZABLE);
    window.SetMinSize(320, static_cast<int>(320.0f / board.getAspectRatio()));
    window.SetTargetFPS(60);

    board.loadTexture();
    board.updateScale(GetScreenWidth());

    // ── textures ──
    Sprite gooseTex("assets/goose_day_0.png");
    gooseTex.loadTexture();
    const raylib::Texture* gooseRaw = &gooseTex.getTexture();

    // ── game state ──
    std::vector<Tower> towers;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    int lives = 20;
    int gold  = 200;
    int score = 0;

    // ── wave manager ──
    WaveManager waveMgr(board);
    // C++17 — no designated initializers
    auto addW = [&](int cnt, float interv, int hp, float spd, int rew) {
        WaveDef w;
        w.enemyCount    = cnt;
        w.spawnInterval = interv;
        w.hp            = hp;
        w.speed         = spd;
        w.reward        = rew;
        waveMgr.addWave(w);
    };
    addW(5,  1.5f,  80,  55.0f, 30);  // wave 1
    addW(8,  1.2f, 100, 60.0f, 35);  // wave 2
    addW(12, 1.0f, 120, 65.0f, 40);  // wave 3
    addW(15, 0.9f, 150, 70.0f, 45);  // wave 4
    addW(20, 0.8f, 200, 75.0f, 50);  // wave 5

    waveMgr.start();

    // Cache lane waypoints per row (rebuilt on resize)
    std::vector<std::vector<raylib::Vector2>> laneWps(board.getRowCount());
    auto rebuildWaypoints = [&]() {
        for (int r = 0; r < board.getRowCount(); ++r)
            laneWps[r] = buildLaneWaypoints(board, r);
    };
    rebuildWaypoints();

    // ── main loop ──
    while (!window.ShouldClose()) {
        // -- aspect-ratio lock --
        if (IsWindowResized()) {
            const int curW = GetScreenWidth();
            const int curH = GetScreenHeight();
            const float curAspect =
                static_cast<float>(curW) / static_cast<float>(curH);
            if (std::abs(curAspect - board.getAspectRatio()) > 0.001f) {
                const int clampedH =
                    static_cast<int>(std::round(static_cast<float>(curW) / board.getAspectRatio()));
                SetWindowSize(curW, clampedH);
            }
            board.updateScale(GetScreenWidth());
            rebuildWaypoints();
        }

        const float dt = GetFrameTime();

        // ── input: place tower ──
        {
            const int hovered = board.hoveredCell(GetMousePosition());
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered >= 0) {
                const int col = hovered % board.getColCount();
                const int row = hovered / board.getColCount();

                bool occupied = false;
                for (const auto& t : towers) {
                    if (t.getCol() == col && t.getRow() == row) {
                        occupied = true;
                        break;
                    }
                }
                if (!occupied && gold >= 100) {
                    towers.emplace_back(col, row, 250.0f, 0.8f, 25);
                    gold -= 100;
                }
            }
        }

        // ── update: wave spawn ──
        {
            Enemy* rawEnemy = waveMgr.update(dt);
            if (rawEnemy) {
                enemies.push_back(std::unique_ptr<Enemy>(rawEnemy));
            }
        }

        // ── update: towers → fire projectiles ──
        std::vector<Enemy*> enemyPtrs;
        for (auto& e : enemies) enemyPtrs.push_back(e.get());

        for (auto& tow : towers) {
            const auto cellR = board.cellRect(tow.getCol(), tow.getRow());
            const raylib::Vector2 towerPos(
                static_cast<float>(cellR.x + cellR.w / 2),
                static_cast<float>(cellR.y + cellR.h / 2));
            Projectile* p = tow.update(dt, towerPos, enemyPtrs);
            if (p) projectiles.push_back(std::unique_ptr<Projectile>(p));
        }

        // ── update: projectiles ──
        for (auto& p : projectiles) p->update(dt);

        // ── update: enemies ──
        for (auto& e : enemies) {
            if (e->getState() == Enemy::WALKING)
                e->update(dt, laneWps[e->getRow()]);
        }

        // ── cleanup: impacted projectiles ──
        eraseIf(projectiles, [](const auto& p) { return p->hasImpacted(); });

        // ── cleanup: dead / escaped enemies ──
        for (auto& e : enemies) {
            if (e->isDead()) {
                gold  += e->getReward();
                score += e->getReward();
            }
            if (e->isEscaped()) {
                lives = std::max(0, lives - 1);
            }
        }
        eraseIf(enemies, [](const auto& e) { return e->isDead() || e->isEscaped(); });

        // ── wave progression ──
        if (waveMgr.isWaveActive() && enemies.empty()) {
            waveMgr.advanceWave();
        }

        // ── game over check ──
        if (lives <= 0) break;
        if (waveMgr.allWavesDone() && enemies.empty()) {
            // Victory
            break;
        }

        // ── draw ──
        BeginDrawing();
        window.ClearBackground(raylib::Color::RayWhite());

        board.draw();

        // Towers
        for (const auto& tow : towers) {
            const auto r = board.cellRect(tow.getCol(), tow.getRow());
            const raylib::Vector2 pos(
                static_cast<float>(r.x + r.w / 2),
                static_cast<float>(r.y + r.h / 2));
            tow.draw(gooseRaw, pos);
        }

        // Enemies
        for (const auto& e : enemies)
            e->draw(gooseRaw);

        // Projectiles
        for (const auto& p : projectiles)
            p->draw();

        // Hover highlight
        {
            const int hov = board.hoveredCell(GetMousePosition());
            board.drawHover(hov);
        }

        // ── UI overlay ──
        {
            const int fs = 22;
            const int margin = 12;
            const int rowH = fs + 4;

            // Lives (top-left)
            std::string livesStr = "Lives: " + std::to_string(lives);
            raylib::DrawText(livesStr.c_str(), margin, margin, fs, RED);

            // Gold (top-left below lives)
            std::string goldStr = "Gold: " + std::to_string(gold);
            raylib::DrawText(goldStr.c_str(), margin, margin + rowH, fs, GOLD);

            // Wave (top-left below gold)
            std::string waveStr = "Wave " + std::to_string(waveMgr.getCurrentWave() + 1)
                                + " / " + std::to_string(waveMgr.getWaveCount());
            raylib::DrawText(waveStr.c_str(), margin, margin + rowH * 2, fs, DARKGREEN);

            // Score (top-right)
            std::string scoreStr = "Score: " + std::to_string(score);
            const int scoreX = GetScreenWidth() - MeasureText(scoreStr.c_str(), fs) - margin;
            raylib::DrawText(scoreStr.c_str(), scoreX, margin, fs, DARKBLUE);

            // Tower cost hint
            std::string costStr = "[Click cell] Tower: 100g";
            const int costX = GetScreenWidth() - MeasureText(costStr.c_str(), 16) - margin;
            raylib::DrawText(costStr.c_str(), costX, margin + rowH, 16, GRAY);
        }

        EndDrawing();
    }

    // ── game over / victory screen ──
    {
        BeginDrawing();
        window.ClearBackground(raylib::Color::RayWhite());

        const char* msg = (lives <= 0) ? "GAME OVER" : "VICTORY!";
        const int fs = 60;
        const int tw = MeasureText(msg, fs);
        raylib::DrawText(msg, (GetScreenWidth() - tw) / 2,
                          GetScreenHeight() / 2 - 40, fs, (lives <= 0) ? RED : GREEN);

        std::string finalScore = "Final Score: " + std::to_string(score);
        raylib::DrawText(finalScore.c_str(),
                          (GetScreenWidth() - MeasureText(finalScore.c_str(), 30)) / 2,
                          GetScreenHeight() / 2 + 30, 30, DARKGRAY);

        EndDrawing();
        while (!window.ShouldClose()) {}
    }

    return 0;
}
