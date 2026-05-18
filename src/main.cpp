#include "board.hpp"
#include "sprite.hpp"

#include <cmath>
#include <utility>
#include <vector>

int main() {
    Board board("assets/map_day_0.png");

    const int kInitW = static_cast<int>(720.0f * board.getAspectRatio());
    const int kInitH = 720;

    raylib::Window window(kInitW, kInitH, "FiboTowerDefense", FLAG_WINDOW_RESIZABLE);
    window.SetMinSize(320, static_cast<int>(320.0f / board.getAspectRatio()));
    window.SetTargetFPS(60);

    board.loadTexture();
    board.updateScale(GetScreenWidth());

    // Goose sprite shared by all placed geese
    Sprite goose("assets/goose_day_0.png");
    goose.loadTexture();

    // Placed tower positions (col, row)
    std::vector<std::pair<int, int>> towers;

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
        }

        // -- input --
        const int hovered = board.hoveredCell(GetMousePosition());

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered >= 0) {
            const int col = hovered % board.getColCount();
            const int row = hovered / board.getColCount();

            // Check not already occupied
            bool occupied = false;
            for (const auto& t : towers) {
                if (t.first == col && t.second == row) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                towers.emplace_back(col, row);
            }
        }

        // -- draw --
        BeginDrawing();
        window.ClearBackground(raylib::Color::RayWhite());

        board.draw();
        board.drawHover(hovered);

        // Draw all placed geese (fitted to cell, keep aspect)
        for (const auto& [col, row] : towers) {
            const auto [x, y, w, h] = board.cellRect(col, row);
            goose.drawFitted(x, y, w, h);
        }

        EndDrawing();
    }

    return 0;
}
