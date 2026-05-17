#include "raylib-cpp.hpp"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "FiboTowerDefense");
    window.SetTargetFPS(60);

    while (!window.ShouldClose()) {
        BeginDrawing();
        window.ClearBackground(raylib::Color::RayWhite());

        raylib::DrawText("FiboTowerDefense - raylib-cpp", 190, 200, 20, raylib::Color::LightGray());
        raylib::DrawText("Press ESC to exit", 250, 250, 15, raylib::Color::Gray());

        EndDrawing();
    }

    return 0;
}
