#include "board.hpp"

#include <cmath>

Board::Board(const std::string& bgPath) {
    raylib::Image img(bgPath);
    imageW_ = static_cast<float>(img.GetWidth());
    imageH_ = static_cast<float>(img.GetHeight());

    // Boost bg vibrancy (CPU-side, safe before Window)
    img.ColorContrast(1.3f);
    img.ColorBrightness(8);

    // Keep processed image for later GPU upload
    processedImage_ = img;
    recacheRects();
}

void Board::loadTexture() {
    background_.Load(processedImage_);  // GPU upload (needs GL context)
    processedImage_.Unload();           // CPU copy no longer needed
    background_.SetFilter(TEXTURE_FILTER_POINT);
}

void Board::updateScale(int screenWidth) {
    scale_ = static_cast<float>(screenWidth) / imageW_;
    recacheRects();
}

void Board::recacheRects() {
    for (int r = 0; r < kRows; ++r)
        for (int c = 0; c < kCols; ++c)
            cellRects_[r][c] = {
                static_cast<int>(std::round((kOriginX + static_cast<float>(c) * kCellW) * scale_)),
                static_cast<int>(std::round((kOriginY + static_cast<float>(r) * kCellH) * scale_)),
                static_cast<int>(std::round(kCellW * scale_)),
                static_cast<int>(std::round(kCellH * scale_))
            };
}

raylib::Vector2 Board::screenToImage(raylib::Vector2 screen) const {
    return {screen.x / scale_, screen.y / scale_};
}

int Board::hoveredCell(raylib::Vector2 mouse) const {
    const raylib::Vector2 img = screenToImage(mouse);
    if (img.x < kOriginX || img.y < kOriginY) return -1;

    const int col = static_cast<int>((img.x - kOriginX) / kCellW);
    const int row = static_cast<int>((img.y - kOriginY) / kCellH);
    if (col < 0 || col >= kCols || row < 0 || row >= kRows) return -1;

    return col + row * kCols;
}

void Board::draw() const {
    const Rectangle src{0.0f, 0.0f, imageW_, imageH_};
    const Rectangle dst{0.0f, 0.0f,
                        static_cast<float>(GetScreenWidth()),
                        static_cast<float>(GetScreenHeight())};
    background_.Draw(src, dst, {}, 0.0f, raylib::Color::White());
}

void Board::drawHover(int cellIndex) const {
    if (cellIndex < 0) return;

    const int col = cellIndex % kCols;
    const int row = cellIndex / kCols;
    const auto [x, y, w, h] = cellRect(col, row);

    DrawRectangle(x, y, w, h, {255, 220, 100, 55});
}
