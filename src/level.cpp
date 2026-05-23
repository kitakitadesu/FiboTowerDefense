#include "level.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>

#include <raygui.h>

Level::Level(Board& grid, Tower& tower, Scoreboard& scoreboard)
    : id_(IdGenerator::getNextId()), grid_(grid), tower_(tower), scoreboard_(scoreboard),
      waveMgr_(grid)
{
    listenToEvents();

    sfxSelect_ = LoadSound("assets/select_006.ogg");
    sfxPlace_  = LoadSound("assets/drop_001.ogg");
    sfxError_  = LoadSound("assets/error_008.ogg");
}
Level::~Level() {
    UnloadSound(sfxSelect_);
    UnloadSound(sfxPlace_);
    UnloadSound(sfxError_);
}

void Level::listenToEvents() {
    events_.sink<EnemyKilledEvent>().connect<&Level::onEnemyKilled>(this);
}

void Level::onEnemyKilled(const EnemyKilledEvent& event) {
    (void)event;
}

bool Level::spendCurrency(int amt) {
    if (currency_ < amt) return false;
    currency_ -= amt;
    return true;
}

void Level::addEnemy(std::unique_ptr<Enemy> e) {
    enemies_.push_back(std::move(e));
}

void Level::start() {
    waveMgr_.start();
    totalEnemiesThisWave_ = waveMgr_.getCurrentWaveEnemyCount();
}

void Level::update(float dt, const std::vector<std::vector<raylib::Vector2>>& laneWps) {
    // ── wave spawn ──
    {
        auto newEnemy = waveMgr_.update(dt);
        if (newEnemy) {
            const int row = newEnemy->getRow();
            if (row >= 0 && row < static_cast<int>(laneWps.size()) && !laneWps[row].empty())
                newEnemy->spawnAt(laneWps[row].front());
            enemies_.push_back(std::move(newEnemy));
        }
    }

    // ── turrets fire ──
    {
        std::vector<Enemy*> enemyPtrs;
        for (auto& e : enemies_) enemyPtrs.push_back(e.get());

        for (auto& tur : turrets_) {
            const auto cellR = grid_.cellRect(tur.getCol(), tur.getRow());
            const raylib::Vector2 turretPos(
                static_cast<float>(cellR.x + cellR.w / 2),
                static_cast<float>(cellR.y + cellR.h / 2));
            auto p = tur.update(dt, turretPos, enemyPtrs);
            if (p) projectiles_.push_back(std::move(p));
        }
    }

    // ── solar cells → gold ──
    for (auto& sc : solarCells_) {
        int earned = sc.update(dt);
        if (earned > 0) {
            currency_ += earned;
            const auto cr = grid_.cellRect(sc.getCol(), sc.getRow());
            floatingTexts_.push_back({
                {static_cast<float>(cr.x + cr.w / 2), static_cast<float>(cr.y - 10)},
                "+" + std::to_string(earned) + "g", GREEN, 1.2f, 1.2f, {0.0f, -40.0f}
            });
        }
    }

    // ── projectiles ──
    for (auto& p : projectiles_) p->update(dt);

    // ── enemies — collision with turrets / solar ──
    const float enemyDPS = 10.0f;
    for (auto& e : enemies_) {
        if (e->getState() != Enemy::WALKING) continue;

        bool blocked = false;
        for (auto& t : turrets_) {
            if (!t.isAlive() || t.getRow() != e->getRow()) continue;
            const auto cellR = grid_.cellRect(t.getCol(), t.getRow());
            const float tx = static_cast<float>(cellR.x + cellR.w / 2);
            const float cellHalfW = cellR.w * 0.4f;
            const float dx = tx - e->getPosition().x;
            if (dx > 0 && dx < cellHalfW + e->getRadius()) {
                blocked = true;
                t.takeDamage(enemyDPS * dt);
                break;
            }
        }

        if (!blocked) {
            for (const auto& sc : solarCells_) {
                if (sc.getRow() != e->getRow()) continue;
                const auto cellR = grid_.cellRect(sc.getCol(), sc.getRow());
                const float sx = static_cast<float>(cellR.x + cellR.w / 2);
                const float cellHalfW = cellR.w * 0.4f;
                const float dx = sx - e->getPosition().x;
                if (dx > 0 && dx < cellHalfW + e->getRadius()) {
                    blocked = true;
                    break;
                }
            }
        }

        if (!blocked)
            e->update(dt, laneWps[e->getRow()]);
    }

    // ── cleanup impacted projectiles ──
    eraseIf(projectiles_, [](const auto& p) { return p->hasImpacted(); });

    // ── cleanup dead / escaped enemies ──
    for (auto& e : enemies_) {
        if (e->isDead()) {
            currency_ += e->getReward();
            scoreboard_.addPoints(e->getReward());
            ++enemiesKilledThisWave_;
            floatingTexts_.push_back({
                e->getPosition(),
                "+" + std::to_string(e->getReward()),
                GOLD, 1.0f, 1.0f, {0.0f, -40.0f}
            });
            events_.trigger(EnemyKilledEvent{e->getPosition()});
        }
        if (e->isEscaped()) {
            if (!cheatMode_)
                tower_.takeDamage(1);
        }
    }
    eraseIf(enemies_, [](const auto& e) { return e->isDead() || e->isEscaped(); });

    // ── cleanup destroyed turrets ──
    {
        int idx = 0;
        auto it = turrets_.begin();
        while (it != turrets_.end()) {
            if (!it->isAlive()) {
                if (idx == selectedTurretIdx_) selectedTurretIdx_ = -1;
                const auto cr = grid_.cellRect(it->getCol(), it->getRow());
                floatingTexts_.push_back({
                    {static_cast<float>(cr.x + cr.w / 2), static_cast<float>(cr.y)},
                    "Turret Lost!", RED, 1.5f, 1.5f, {0.0f, -60.0f}
                });
                it = turrets_.erase(it);
                // idx stays same — next element shifts down
            } else {
                ++it;
                ++idx;
            }
        }
    }

    // ── floating texts ──
    for (auto& ft : floatingTexts_) {
        ft.pos.x += ft.velocity.x * dt;
        ft.pos.y += ft.velocity.y * dt;
        ft.life -= dt;
    }
    eraseIf(floatingTexts_, [](const auto& ft) { return ft.life <= 0.0f; });

    // ── wave announcement timer ──
    if (waveAnnounce_) {
        waveAnnounce_->life -= dt;
        if (waveAnnounce_->life <= 0.0f) waveAnnounce_.reset();
    }

    // ── sell confirm timer (auto-cancel after 5s) ──
    sellConfirmTimer_ -= dt;
    if (sellConfirmTimer_ <= 0.0f) { sellConfirmIdx_ = -1; }

    // ── wave progression ──
    if (waveMgr_.isWaveActive() && enemies_.empty()) {
        waveMgr_.advanceWave();
        const int nextWave = waveMgr_.getCurrentWave() + 1;
        const int totalWaves = waveMgr_.getWaveCount();

        // Announce "Wave N!" (or "Final Wave!" on last wave)
        if (nextWave == totalWaves) {
            waveAnnounce_ = {"FINAL WAVE!", 3.0f, 3.0f};
        } else {
            waveAnnounce_ = {"Wave " + std::to_string(nextWave) + "!", 3.0f, 3.0f};
        }

        enemiesKilledThisWave_ = 0;
        totalEnemiesThisWave_ = waveMgr_.getCurrentWaveEnemyCount();
        selectedTurretIdx_ = -1;
    }
}

void Level::render(const raylib::Texture* gooseRaw) {
    grid_.draw();

    // Tower base
    tower_.draw();

    // Turrets
    for (const auto& tur : turrets_) {
        const auto r = grid_.cellRect(tur.getCol(), tur.getRow());
        const raylib::Vector2 pos(
            static_cast<float>(r.x + r.w / 2),
            static_cast<float>(r.y + r.h / 2));
        tur.draw(gooseRaw, pos);
    }

    // Solar cells
    for (const auto& sc : solarCells_) {
        const auto r = grid_.cellRect(sc.getCol(), sc.getRow());
        sc.draw(gooseRaw, {static_cast<float>(r.x + r.w / 2), static_cast<float>(r.y + r.h / 2)});
    }

    // Enemies
    for (const auto& e : enemies_)
        e->draw(gooseRaw);

    // Projectiles
    for (const auto& p : projectiles_)
        p->draw();

}

void Level::renderUI() {
    const int scrW = GetScreenWidth();
    const int scrH = GetScreenHeight();
    const int panelW = 270;

    // ── Build mode keyboard shortcuts ──
    if (IsKeyPressed(KEY_T)) placingMode_ = BuildMode::ShootTurret;
    if (IsKeyPressed(KEY_M)) placingMode_ = BuildMode::MeleeTurret;
    if (IsKeyPressed(KEY_S)) placingMode_ = BuildMode::SolarCell;

    // ── Right-click cancels build mode ──
    if (placingMode_ != BuildMode::None && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        placingMode_ = BuildMode::None;
    }

    // ── Build toolbar (bottom center) ──
    {
        constexpr int kBtns   = 3;
        constexpr int kBtnW   = 180;
        constexpr int kBtnH   = 40;
        constexpr int kPad    = 8;
        constexpr int kBarW   = kBtns * (kBtnW + kPad) - kPad + kPad * 2;
        constexpr int kBarH   = kBtnH + kPad * 2;
        const int kBarX       = (scrW - kBarW) / 2;
        const int kBarY       = scrH - kBarH - 10;

        DrawRectangleRounded({static_cast<float>(kBarX), static_cast<float>(kBarY),
                              static_cast<float>(kBarW), static_cast<float>(kBarH)},
                             0.2f, 8, {0, 0, 0, 200});

        // Button helper with active highlight
        auto buildBtn = [&](int idx, const char* label, BuildMode mode, bool affordable) {
            const int bx = kBarX + kPad + idx * (kBtnW + kPad);
            const int by = kBarY + kPad;
            const bool active = (placingMode_ == mode);
            if (!affordable && !active) GuiSetState(STATE_DISABLED);

            if (active) {
                int ob = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
                int ot = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);
                GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, (int)ColorToInt(Color{60, 140, 255, 255}));
                GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, (int)ColorToInt(WHITE));
                if (GuiButton({static_cast<float>(bx), static_cast<float>(by),
                               static_cast<float>(kBtnW), static_cast<float>(kBtnH)}, label)) {
                    PlaySound(sfxSelect_);
                    placingMode_ = BuildMode::None;
                }
                GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ob);
                GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ot);
            } else {
                if (GuiButton({static_cast<float>(bx), static_cast<float>(by),
                               static_cast<float>(kBtnW), static_cast<float>(kBtnH)}, label)) {
                    PlaySound(sfxSelect_);
                    placingMode_ = mode;
                }
            }

            GuiSetState(STATE_NORMAL);
        };

        const bool affordableShoot = currency_ >= 100;
        const bool affordableMelee = currency_ >= 80;
        const bool affordableSolar = currency_ >= 60;

        buildBtn(0, "Tower  (T)  100g", BuildMode::ShootTurret, affordableShoot);
        buildBtn(1, "Melee  (M)  80g",  BuildMode::MeleeTurret, affordableMelee);
        buildBtn(2, "Solar  (S)  60g",  BuildMode::SolarCell,   affordableSolar);
    }

    // ── Unified click handler: select existing or place new ──
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Skip grid handling when clicking on a visible detail panel
        {
            const raylib::Vector2 mp = GetMousePosition();
            if (selectedTurretIdx_ >= 0 && selectedTurretIdx_ < static_cast<int>(turrets_.size())) {
                auto& selT = turrets_[selectedTurretIdx_];
                const auto sr = grid_.cellRect(selT.getCol(), selT.getRow());
                const int cx = scrW / 2;
                int px = (sr.x < cx) ? sr.x + sr.w + 12 : sr.x - panelW - 12;
                int py = sr.y;
                if (px < 4) px = 4;
                if (px + panelW > scrW - 4) px = scrW - panelW - 4;
                if (py < 4) py = 4;
                if (py + 240 > scrH - 70) py = scrH - 70 - 240;
                if (mp.x >= px && mp.x <= px + panelW && mp.y >= py && mp.y <= py + 240) goto clickHandled;
            }
            if (selectedSolarIdx_ >= 0 && selectedSolarIdx_ < static_cast<int>(solarCells_.size())) {
                auto& selS = solarCells_[selectedSolarIdx_];
                const auto sr = grid_.cellRect(selS.getCol(), selS.getRow());
                const int cx = scrW / 2;
                int px = (sr.x < cx) ? sr.x + sr.w + 12 : sr.x - panelW - 12;
                int py = sr.y;
                if (px < 4) px = 4;
                if (px + panelW > scrW - 4) px = scrW - panelW - 4;
                if (py < 4) py = 4;
                if (py + 210 > scrH - 70) py = scrH - 70 - 210;
                if (mp.x >= px && mp.x <= px + panelW && mp.y >= py && mp.y <= py + 210) goto clickHandled;
            }
        }

        const int hovered = grid_.hoveredCell(GetMousePosition());
        if (hovered >= 0) {
            const int col = hovered % grid_.getColCount();
            const int row = hovered / grid_.getColCount();

            // Check if an entity exists at this cell
            int foundTurret = -1;
            int foundSolar  = -1;
            for (int i = 0; i < static_cast<int>(turrets_.size()); ++i) {
                if (turrets_[i].getCol() == col && turrets_[i].getRow() == row) {
                    foundTurret = i; break;
                }
            }
            if (foundTurret < 0) {
                for (int i = 0; i < static_cast<int>(solarCells_.size()); ++i) {
                    if (solarCells_[i].getCol() == col && solarCells_[i].getRow() == row) {
                        foundSolar = i; break;
                    }
                }
            }
            if ((foundTurret >= 0 || foundSolar >= 0) && placingMode_ != BuildMode::None) {
                PlaySound(sfxError_); 
            }

            if (foundTurret >= 0 || foundSolar >= 0) {
                // Clicked existing entity → always select regardless of build mode
                PlaySound(sfxSelect_);
                selectedTurretIdx_ = foundTurret;
                selectedSolarIdx_  = foundSolar;
            } else if (placingMode_ != BuildMode::None) {
                // Clicked empty cell while in build mode → place new
                selectedTurretIdx_ = -1;
                selectedSolarIdx_  = -1;
                int cost = 0;
                if (placingMode_ == BuildMode::ShootTurret) cost = 100;
                else if (placingMode_ == BuildMode::MeleeTurret) cost = 80;
                else if (placingMode_ == BuildMode::SolarCell) cost = 60;
                if (currency_ >= cost) {
                    currency_ -= cost;
                    PlaySound(sfxPlace_);

                    if (placingMode_ == BuildMode::ShootTurret)
                        addTurret(Turret(col, row, TurretType::Shooting, 0, 1.5f, 25));
                    else if (placingMode_ == BuildMode::MeleeTurret)
                        addTurret(Turret(col, row, TurretType::Melee, 120, 1.2f, 35));
                    else if (placingMode_ == BuildMode::SolarCell)
                        addSolarCell(SolarCell(col, row));
                } else {
                    PlaySound(sfxError_);

                    raylib::Vector2 mp = GetMousePosition();
                    floatingTexts_.push_back({{mp.x, mp.y - 20},
                        "Not enough gold!", RED, 1.2f, 1.2f, {0.0f, -30.0f}});
                }
            } else {
                // Clicked empty cell, not in build mode → deselect
                selectedTurretIdx_ = -1;
                selectedSolarIdx_  = -1;
            }
        }
    }
clickHandled:
    ;

    // ── Turret selected ──
    if (selectedTurretIdx_ >= 0 && selectedTurretIdx_ < static_cast<int>(turrets_.size())) {
        auto& selT = turrets_[selectedTurretIdx_];
        const auto sr = grid_.cellRect(selT.getCol(), selT.getRow());
        DrawRectangleLinesEx({static_cast<float>(sr.x), static_cast<float>(sr.y),
                              static_cast<float>(sr.w), static_cast<float>(sr.h)}, 3, GOLD);

        const int cx = scrW / 2;
        int px = (sr.x < cx) ? sr.x + sr.w + 12 : sr.x - panelW - 12;
        int py = sr.y;
        if (px < 4) px = 4;
        if (px + panelW > scrW - 4) px = scrW - panelW - 4;
        if (py < 4) py = 4;
        if (py + 240 > scrH - 70) py = scrH - 70 - 240;
        const int pH = 220;

        DrawRectangleRounded({static_cast<float>(px), static_cast<float>(py),
                              static_cast<float>(panelW), static_cast<float>(pH)}, 0.2f, 10, {20, 20, 20, 235});

        const char* tn = (selT.getTurretType() == TurretType::Shooting) ? "Shooting Turret" : "Melee Turret";
        std::string title = std::string(tn) + "  Lv." + std::to_string(selT.getLevel());
        const int tFs = 17;
        raylib::DrawText(title.c_str(), px + panelW/2 - MeasureText(title.c_str(), tFs)/2, py + 10, tFs, WHITE);

        int rowY = py + 40;
        const int lx = px + 14, vx = px + 105, ax = px + 173, nx = px + 195, rh = 21, sFs = 14;
        char b1[16], b2[16];
        auto drawStat = [&](const char* lbl, const char* cur, const char* nxt) {
            raylib::DrawText(lbl, lx, rowY, sFs, LIGHTGRAY);
            raylib::DrawText(cur, vx, rowY, sFs, WHITE);
            if (nxt) { DrawText("\xE2\x86\x92", ax, rowY, sFs, GRAY); raylib::DrawText(nxt, nx, rowY, sFs, GREEN); }
            rowY += rh;
        };

        snprintf(b1, sizeof(b1), "%d", selT.getDamage());
        if (!selT.isMaxLevel()) { snprintf(b2, sizeof(b2), "%d", selT.getDamage() + 10); drawStat("Damage", b1, b2); }
        else drawStat("Damage", b1, nullptr);

        snprintf(b1, sizeof(b1), "%.1f/s", selT.getFireRate());
        if (!selT.isMaxLevel()) { snprintf(b2, sizeof(b2), "%.1f/s", selT.getFireRate() + 0.1f); drawStat("Fire Rate", b1, b2); }
        else drawStat("Fire Rate", b1, nullptr);

        if (selT.getTurretType() == TurretType::Shooting) drawStat("Range", "\xE2\x88\x9E", nullptr);
        else {
            snprintf(b1, sizeof(b1), "%.0f", selT.getRange());
            if (!selT.isMaxLevel()) { snprintf(b2, sizeof(b2), "%.0f", selT.getRange() + 20.0f); drawStat("Range", b1, b2); }
            else drawStat("Range", b1, nullptr);
        }

        const int btnY = rowY + 6;
        if (!selT.isMaxLevel()) {
            const int cost = selT.getUpgradeCost();
            Rectangle ub{static_cast<float>(px + 14), static_cast<float>(btnY), 242.0f, 30.0f};
            char lbl[32]; snprintf(lbl, sizeof(lbl), "UPGRADE  (%dg)", cost);
            if (currency_ < cost) GuiSetState(STATE_DISABLED);
            if (GuiButton(ub, lbl) && currency_ >= cost) { currency_ -= cost; turrets_[selectedTurretIdx_].upgrade(); }
            GuiSetState(STATE_NORMAL);
            Rectangle sb{static_cast<float>(px + 14), static_cast<float>(btnY + 35), 242.0f, 30.0f};
            const int sRefund = (selT.getTurretType() == TurretType::Shooting) ? 50 * selT.getLevel() : 40 * selT.getLevel();
            snprintf(lbl, sizeof(lbl), "SELL  (%dg)", sRefund);
            if (GuiButton(sb, lbl)) {
                sellConfirmIdx_ = selectedTurretIdx_;
                sellConfirmIsTurret_ = true;
                sellConfirmGold_ = sRefund;
                sellConfirmTimer_ = 5.0f;
                sellConfirmSelCol_ = selT.getCol();
                sellConfirmSelRow_ = selT.getRow();
            }
        } else {
            raylib::DrawText("MAX LEVEL", px + panelW/2 - MeasureText("MAX LEVEL", 16)/2, btnY + 6, 16, GOLD);
            Rectangle sb{static_cast<float>(px + 14), static_cast<float>(btnY + 30), 242.0f, 30.0f};
            const int sRefund = (selT.getTurretType() == TurretType::Shooting) ? 50 * selT.getLevel() : 40 * selT.getLevel();
            char lbl[32]; snprintf(lbl, sizeof(lbl), "SELL  (%dg)", sRefund);
            if (GuiButton(sb, lbl)) {
                sellConfirmIdx_ = selectedTurretIdx_;
                sellConfirmIsTurret_ = true;
                sellConfirmGold_ = sRefund;
                sellConfirmTimer_ = 5.0f;
                sellConfirmSelCol_ = selT.getCol();
                sellConfirmSelRow_ = selT.getRow();
            }
        }
        // ── X close button (top-right corner) ──
        {
            constexpr int kBtnS = 24;
            const int xBtnX = px + panelW - kBtnS - 6;
            const int xBtnY = py + 5;
            if (GuiButton({static_cast<float>(xBtnX), static_cast<float>(xBtnY),
                           static_cast<float>(kBtnS), static_cast<float>(kBtnS)}, "X")) {
                selectedTurretIdx_ = -1;
            }
        }
    }

    // ── Solar cell selected ──
    if (selectedSolarIdx_ >= 0 && selectedSolarIdx_ < static_cast<int>(solarCells_.size())) {
        auto& selS = solarCells_[selectedSolarIdx_];
        const auto sr = grid_.cellRect(selS.getCol(), selS.getRow());
        DrawRectangleLinesEx({static_cast<float>(sr.x), static_cast<float>(sr.y),
                              static_cast<float>(sr.w), static_cast<float>(sr.h)}, 3, GOLD);

        const int cx = scrW / 2;
        int px = (sr.x < cx) ? sr.x + sr.w + 12 : sr.x - panelW - 12;
        int py = sr.y;
        if (px < 4) px = 4;
        if (px + panelW > scrW - 4) px = scrW - panelW - 4;
        if (py < 4) py = 4;
        if (py + 210 > scrH - 70) py = scrH - 70 - 210;
        const int pH = 190;

        DrawRectangleRounded({static_cast<float>(px), static_cast<float>(py),
                              static_cast<float>(panelW), static_cast<float>(pH)}, 0.2f, 10, {20, 20, 20, 235});

        std::string title = "Solar Cell  Lv." + std::to_string(selS.getLevel());
        const int tFs = 17;
        raylib::DrawText(title.c_str(), px + panelW/2 - MeasureText(title.c_str(), tFs)/2, py + 10, tFs, WHITE);

        int rowY = py + 40;
        const int lx = px + 14, vx = px + 105, ax = px + 173, nx = px + 195, rh = 21, sFs = 14;
        char b1[16], b2[16];
        auto drawStat = [&](const char* lbl, const char* cur, const char* nxt) {
            raylib::DrawText(lbl, lx, rowY, sFs, LIGHTGRAY);
            raylib::DrawText(cur, vx, rowY, sFs, WHITE);
            if (nxt) { DrawText("\xE2\x86\x92", ax, rowY, sFs, GRAY); raylib::DrawText(nxt, nx, rowY, sFs, GREEN); }
            rowY += rh;
        };

        snprintf(b1, sizeof(b1), "%dg", selS.getReward());
        if (!selS.isMaxLevel()) { snprintf(b2, sizeof(b2), "%dg", selS.getReward() + 3); drawStat("Reward", b1, b2); }
        else drawStat("Reward", b1, nullptr);

        snprintf(b1, sizeof(b1), "%.1fs", selS.getInterval());
        if (!selS.isMaxLevel()) { snprintf(b2, sizeof(b2), "%.1fs", selS.getInterval() - 0.2f); drawStat("Interval", b1, b2); }
        else drawStat("Interval", b1, nullptr);

        const int btnY = rowY + 6;
        if (!selS.isMaxLevel()) {
            const int cost = selS.getUpgradeCost();
            Rectangle ub{static_cast<float>(px + 14), static_cast<float>(btnY), 242.0f, 30.0f};
            char lbl[32]; snprintf(lbl, sizeof(lbl), "UPGRADE  (%dg)", cost);
            if (currency_ < cost) GuiSetState(STATE_DISABLED);
            if (GuiButton(ub, lbl) && currency_ >= cost) { currency_ -= cost; solarCells_[selectedSolarIdx_].upgrade(); }
            GuiSetState(STATE_NORMAL);
            Rectangle sb{static_cast<float>(px + 14), static_cast<float>(btnY + 35), 242.0f, 30.0f};
            const int sRefund = selS.getSellRefund();
            snprintf(lbl, sizeof(lbl), "SELL  (%dg)", sRefund);
            if (GuiButton(sb, lbl)) {
                sellConfirmIdx_ = selectedSolarIdx_;
                sellConfirmIsTurret_ = false;
                sellConfirmGold_ = sRefund;
                sellConfirmTimer_ = 5.0f;
                sellConfirmSelCol_ = selS.getCol();
                sellConfirmSelRow_ = selS.getRow();
            }
        } else {
            raylib::DrawText("MAX LEVEL", px + panelW/2 - MeasureText("MAX LEVEL", 16)/2, btnY + 6, 16, GOLD);
            Rectangle sb{static_cast<float>(px + 14), static_cast<float>(btnY + 30), 242.0f, 30.0f};
            const int sRefund = selS.getSellRefund();
            char lbl[32]; snprintf(lbl, sizeof(lbl), "SELL  (%dg)", sRefund);
            if (GuiButton(sb, lbl)) {
                sellConfirmIdx_ = selectedSolarIdx_;
                sellConfirmIsTurret_ = false;
                sellConfirmGold_ = sRefund;
                sellConfirmTimer_ = 5.0f;
                sellConfirmSelCol_ = selS.getCol();
                sellConfirmSelRow_ = selS.getRow();
            }
        }
        // ── X close button (top-right corner) ──
        {
            constexpr int kBtnS = 24;
            const int xBtnX = px + panelW - kBtnS - 6;
            const int xBtnY = py + 5;
            if (GuiButton({static_cast<float>(xBtnX), static_cast<float>(xBtnY),
                           static_cast<float>(kBtnS), static_cast<float>(kBtnS)}, "X")) {
                selectedSolarIdx_ = -1;
            }
        }
    }

    // ── Hover highlight & ghost (skip when detail panel open) ──
    const bool detailOpen = (selectedTurretIdx_ >= 0 || selectedSolarIdx_ >= 0);
    if (!detailOpen) {
        const int hov = grid_.hoveredCell(GetMousePosition());
        grid_.drawHover(hov);

        if (hov >= 0) {
            const int col = hov % grid_.getColCount();
            const int row = hov / grid_.getColCount();

            bool occupied = false;
            for (const auto& t : turrets_) {
                if (t.getCol() == col && t.getRow() == row) { occupied = true; break; }
            }
            if (!occupied) {
                for (const auto& s : solarCells_) {
                    if (s.getCol() == col && s.getRow() == row) { occupied = true; break; }
                }
            }

            const auto cellR = grid_.cellRect(col, row);
            const raylib::Vector2 cellPos(
                static_cast<float>(cellR.x + cellR.w / 2),
                static_cast<float>(cellR.y + cellR.h / 2)
            );

            if (!occupied) {
                const float s = 50.0f;
                const float h = s / 2.0f;
                // Ghost preview: colored by build mode, white if just inspecting
                if (placingMode_ != BuildMode::None || (selectedTurretIdx_ < 0 && selectedSolarIdx_ < 0)) {
                    Color ghostTint;
                    switch (placingMode_) {
                        case BuildMode::ShootTurret: ghostTint = {100, 150, 255, 100}; break;
                        case BuildMode::MeleeTurret: ghostTint = {255, 180, 80, 100};  break;
                        case BuildMode::SolarCell:   ghostTint = {255, 255, 80, 100};  break;
                        default:                     ghostTint = {255, 255, 255, 80};  break;
                    }
                    Rectangle ghostRect = {cellPos.x - h, cellPos.y - h, s, s};
                    DrawRectangleRoundedLinesEx(ghostRect, 0.2f, 6, 2.0f, ghostTint);
                }
            } else {
                for (const auto& t : turrets_) {
                    if (t.getCol() == col && t.getRow() == row && t.getTurretType() == TurretType::Melee) {
                        DrawCircleLines(static_cast<int>(cellPos.x), static_cast<int>(cellPos.y),
                                        120.0f, {255, 255, 255, 150});
                        break;
                    }
                }
            }
        }
    }

    // ── Stats panel (top-left) ──
    {
        const int fs = 22;
        const int sfs = 18;
        const int margin = 15;
        const int rowH = fs + 6;

        const int pw = 235;
        const int ph = margin * 2 + rowH * 4 + 16;
        DrawRectangleRounded({static_cast<float>(margin), static_cast<float>(margin),
                              static_cast<float>(pw), static_cast<float>(ph)}, 0.2f, 10, {0, 0, 0, 180});

        int y = margin + 12;

        // Lives
        std::string livesStr = "\xE2\x99\xA5  " + std::to_string(tower_.getHp()) + "/" + std::to_string(tower_.getMaxHp());
        raylib::DrawText(livesStr.c_str(), margin + 14, y, fs, {255, 80, 80, 255});
        y += rowH;

        // Gold
        std::string goldStr = "$  " + std::to_string(currency_);
        raylib::DrawText(goldStr.c_str(), margin + 14, y, fs, GOLD);
        y += rowH;

        // Wave
        std::string waveStr = "\xE2\x9A\x94  Wave " + std::to_string(waveMgr_.getCurrentWave() + 1)
                            + "/" + std::to_string(waveMgr_.getWaveCount());
        raylib::DrawText(waveStr.c_str(), margin + 14, y, sfs, LIGHTGRAY);
        const int barY = y + 22;
        const int barW = pw - 30;
        const int barH = 10;
        DrawRectangle(margin + 15, barY, barW, barH, {60, 60, 60, 200});
        if (totalEnemiesThisWave_ > 0) {
            float pct = std::min(1.0f, static_cast<float>(enemiesKilledThisWave_) / totalEnemiesThisWave_);
            DrawRectangle(margin + 15, barY, static_cast<int>(barW * pct), barH, SKYBLUE);
        }
        y = barY + barH + 10;

        // Structures
        std::string turretStr = "\xE2\x9B\x94 " + std::to_string(turretsPlaced_) + "  \xE2\x98\x80 " + std::to_string(solarPlaced_);
        raylib::DrawText(turretStr.c_str(), margin + 14, y, sfs, LIGHTGRAY);

        // Score (top-right)
        std::string scoreStr = "\xE2\x98\x85  " + std::to_string(scoreboard_.getCurrentScore());
        const int scoreW = MeasureText(scoreStr.c_str(), fs) + 30;
        const int scoreX = scrW - scoreW - margin;
        DrawRectangleRounded({static_cast<float>(scoreX), static_cast<float>(margin),
                              static_cast<float>(scoreW), static_cast<float>(fs + 30)}, 0.3f, 10, {0, 0, 0, 180});
        raylib::DrawText(scoreStr.c_str(), scoreX + 15, margin + 15, fs, SKYBLUE);
    }

    // ── Sell confirmation dialog ──
    if (sellConfirmIdx_ >= 0) {
        const int scrW = GetScreenWidth();
        const int scrH = GetScreenHeight();
        const int dlgW = 300, dlgH = 140;
        const int dlgX = (scrW - dlgW) / 2, dlgY = (scrH - dlgH) / 2;

        // Dim background
        DrawRectangle(0, 0, scrW, scrH, {0, 0, 0, 120});
        DrawRectangleRounded({static_cast<float>(dlgX), static_cast<float>(dlgY),
                              static_cast<float>(dlgW), static_cast<float>(dlgH)},
                             0.2f, 10, {20, 20, 20, 240});

        char msg[48]; snprintf(msg, sizeof(msg), "Sell for %dg?", sellConfirmGold_);
        raylib::DrawText(msg, dlgX + dlgW/2 - MeasureText(msg, 20)/2, dlgY + 20, 20, WHITE);

        // Yes button
        if (GuiButton({static_cast<float>(dlgX + 25), static_cast<float>(dlgY + 70), 110.0f, 40.0f}, "YES")) {
            // Execute sell
            if (sellConfirmIsTurret_) {
                for (size_t i = 0; i < turrets_.size(); ++i) {
                    if (turrets_[i].getCol() == sellConfirmSelCol_ && turrets_[i].getRow() == sellConfirmSelRow_) {
                        const auto sr = grid_.cellRect(turrets_[i].getCol(), turrets_[i].getRow());
                        currency_ += sellConfirmGold_;
                        floatingTexts_.push_back({{static_cast<float>(sr.x + sr.w/2), static_cast<float>(sr.y)},
                            "+" + std::to_string(sellConfirmGold_) + "g", GOLD, 1.0f, 1.0f, {0.0f, -50.0f}});
                        turrets_.erase(turrets_.begin() + static_cast<int>(i));
                        --turretsPlaced_;
                        break;
                    }
                }
            } else {
                for (size_t i = 0; i < solarCells_.size(); ++i) {
                    if (solarCells_[i].getCol() == sellConfirmSelCol_ && solarCells_[i].getRow() == sellConfirmSelRow_) {
                        const auto sr = grid_.cellRect(solarCells_[i].getCol(), solarCells_[i].getRow());
                        currency_ += sellConfirmGold_;
                        floatingTexts_.push_back({{static_cast<float>(sr.x + sr.w/2), static_cast<float>(sr.y)},
                            "+" + std::to_string(sellConfirmGold_) + "g", GOLD, 1.0f, 1.0f, {0.0f, -50.0f}});
                        solarCells_.erase(solarCells_.begin() + static_cast<int>(i));
                        --solarPlaced_;
                        break;
                    }
                }
            }
            selectedTurretIdx_ = -1;
            selectedSolarIdx_ = -1;
            sellConfirmIdx_ = -1;
        }
        // No button
        if (GuiButton({static_cast<float>(dlgX + 165), static_cast<float>(dlgY + 70), 110.0f, 40.0f}, "NO") ||
            IsKeyPressed(KEY_ESCAPE)) {
            sellConfirmIdx_ = -1;
        }
    }

    // ── Wave announcement ──
    if (waveAnnounce_) {
        const float alpha = std::min(1.0f, waveAnnounce_->life / waveAnnounce_->maxLife * 2.0f);
        const int fontSize = 60;
        const int cx = GetScreenWidth() / 2;
        const int cy = GetScreenHeight() / 3;
        const Color c = {255, 255, 100, static_cast<unsigned char>(alpha * 200)};

        raylib::DrawText(waveAnnounce_->text.c_str(),
            cx - MeasureText(waveAnnounce_->text.c_str(), fontSize) / 2 + 3, cy + 3, fontSize, {0, 0, 0, c.a});
        raylib::DrawText(waveAnnounce_->text.c_str(),
            cx - MeasureText(waveAnnounce_->text.c_str(), fontSize) / 2, cy, fontSize, c);
    }

    // ── Floating texts ──
    for (const auto& ft : floatingTexts_) {
        unsigned char alpha = static_cast<unsigned char>(255.0f * (ft.life / ft.maxLife));
        raylib::Color c = ft.color;
        c.a = alpha;
        raylib::DrawText(ft.text.c_str(), static_cast<int>(ft.pos.x) + 2, static_cast<int>(ft.pos.y) + 2, 20, {0, 0, 0, alpha});
        raylib::DrawText(ft.text.c_str(), static_cast<int>(ft.pos.x), static_cast<int>(ft.pos.y), 20, c);
    }
}
