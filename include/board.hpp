#pragma once

#include <vector>

#include "raylib-cpp.hpp"

/// Screen-space pixel-aligned cell rectangle.
struct CellRect {
    int x, y, w, h;
};

/// Visual game board: background image + 9x5 grid overlay with hover.
class Board {
public:
    explicit Board(const std::string& bgPath);
    ~Board() = default;

    // -- GPU init (call after Window created) --
    void loadTexture();

    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    // -- grid constants (image pixel coords) --
    static constexpr float kOriginX = 395.0f;
    static constexpr float kOriginY = 262.0f;
    static constexpr float kCellW  = 98.0f;
    static constexpr float kCellH  = 118.0f;
    static constexpr int   kCols   = 9;
    static constexpr int   kRows   = 5;

    // -- getters --
    int   getColCount() const { return kCols; }
    int   getRowCount() const { return kRows; }
    float getImageWidth()  const { return imageW_; }
    float getImageHeight() const { return imageH_; }
    float getAspectRatio() const { return imageW_ / imageH_; }

    // -- scale (call when window resized) --
    void updateScale(int screenWidth);

    // -- coordinate transforms --
    raylib::Vector2 screenToImage(raylib::Vector2 screen) const;
    CellRect cellRect(int col, int row) const;

    // -- input --
    /// Returns flat index (col + row * kCols) or -1 if outside grid.
    int hoveredCell(raylib::Vector2 mouse) const;

    // -- draw --
    void draw() const;
    void drawHover(int cellIndex) const;

    // -- cell data --
    char getCell(int row, int col) const;
    void setCellData(int row, int col, char val);

protected:
    bool isValid(int row, int col) const;
    int   numRows_ = kRows;
    int   numCols_ = kCols;
    std::vector<std::vector<char>> data_;

private:
    raylib::Image   processedImage_;
    raylib::Texture background_;
    float imageW_;
    float imageH_;
    float scale_ = 1.0f;
};
