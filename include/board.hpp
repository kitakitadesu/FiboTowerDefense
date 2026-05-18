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
    int cols() const { return kCols; }
    int rows() const { return kRows; }
    float imageWidth()  const { return m_imageW; }
    float imageHeight() const { return m_imageH; }
    float aspectRatio() const { return m_imageW / m_imageH; }

    // -- scale (call when window resized) --
    void updateScale(int screenWidth);

    // -- coordinate transforms --
    raylib::Vector2 screenToImage(Vector2 screen) const;
    CellRect cellRect(int col, int row) const;

    // -- input --
    /// Returns flat index (col + row * kCols) or -1 if outside grid.
    int hoveredCell(Vector2 mouse) const;

    // -- draw --
    void draw() const;
    void drawHover(int cellIndex) const;

    // -- cell data (from reference Board) --
    char getCell(int row, int col) const;
    void setCell(int row, int col, char val);

protected:
    bool isValid(int row, int col) const;
    int   m_numRows = kRows;
    int   m_numCols = kCols;
    std::vector<std::vector<char>> m_data;

private:
    raylib::Image   m_processedImage;
    raylib::Texture m_background;
    float m_imageW;
    float m_imageH;
    float m_scale = 1.0f;
};
