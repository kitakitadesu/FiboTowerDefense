#include "board.hpp"

#include <cmath>

Board::Board() {
    layers_.load("assets");
}

void Board::loadTexture() {
    layers_.upload();
}

void Board::updateScale(int screenWidth) {
    scale_ = static_cast<float>(screenWidth) / layers_.getWidth();
    layers_.setScale(scale_);
}

raylib::Vector2 Board::screenToImage(raylib::Vector2 screen) const {
    return {screen.x / scale_, screen.y / scale_};
}

CellRect Board::cellRect(int col, int row) const {
    return {
        static_cast<int>(std::round((kOriginX + static_cast<float>(col) * kCellW) * scale_)),
        static_cast<int>(std::round((kOriginY + static_cast<float>(row) * kCellH) * scale_)),
        static_cast<int>(std::round(kCellW * scale_)),
        static_cast<int>(std::round(kCellH * scale_))
    };
}

int Board::hoveredCell(raylib::Vector2 mouse) const {
    const raylib::Vector2 img = screenToImage(mouse);
    if (img.x < kOriginX || img.y < kOriginY) return -1;

    const int col = static_cast<int>((img.x - kOriginX) / kCellW);
    const int row = static_cast<int>((img.y - kOriginY) / kCellH);
    if (col < 0 || col >= kCols || row < 0 || row >= kRows) return -1;

    return col + row * kCols;
}

void Board::drawHover(int cellIndex) const {
    if (cellIndex < 0) return;

    const int col = cellIndex % kCols;
    const int row = cellIndex / kCols;
    const auto [x, y, w, h] = cellRect(col, row);

    DrawRectangle(x, y, w, h, {255, 220, 100, 55});
}
