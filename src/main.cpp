#include "game.hpp"

int main() {
    Game game;

    const int kInitW = static_cast<int>(720.0f * game.getBoard().getAspectRatio());
    const int kInitH = 720;

    raylib::Window window(kInitW, kInitH, "FiboTowerDefense", FLAG_WINDOW_RESIZABLE);
    window.SetMinSize(320, static_cast<int>(320.0f / game.getBoard().getAspectRatio()));
    window.SetTargetFPS(60);
    SetExitKey(0);  // disable raylib default ESC→quit; we handle ESC manually

    // เปิดระบบเสียง
    InitAudioDevice();

    // Load GPU textures (must happen after Window creation)
    game.init();

    // ── main loop ──
    while (game.isRunning() && !window.ShouldClose()) {
        const float dt = GetFrameTime();
        game.update(dt);
        game.render();

        // End screen — blocks until quit or restart
        while (game.isRunning() && !window.ShouldClose() &&
               (game.getGameState() == GameState::Won || game.getGameState() == GameState::Lost)) {
            game.renderEndScreen();
            if (game.shouldRestart()) {
                game.resetForRestart();
                break;
            }
        }
    }

    // --- ปิดระบบเสียง ---
    CloseAudioDevice();

    return 0;
}
