#pragma once

#include <string>
#include <vector>

#include "raylib-cpp.hpp"

#include "layer.hpp"

/// Screen-space pixel-aligned cell rectangle.
struct CellRect {
    int x, y, w, h;
};

/// Visual game board: layered background + 9x5 grid overlay with hover.
class Board {
public:
    Board();
    ~Board() = default;

    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    void loadTexture();

    /// Toggle night mode (uses pre-loaded night textures).
    void setNightMode(bool n) { layers_.setNightMode(n); }

    static constexpr float kOriginX = 395.0f;
    static constexpr float kOriginY = 262.0f;
    static constexpr float kCellW  = 98.0f;
    static constexpr float kCellH  = 118.0f;
    static constexpr int   kCols   = 9;
    static constexpr int   kRows   = 5;

    int   getColCount() const { return kCols; }
    int   getRowCount() const { return kRows; }
    float getImageWidth()  const { return layers_.getWidth(); }
    float getImageHeight() const { return layers_.getHeight(); }
    float getAspectRatio() const { return layers_.getAspectRatio(); }

    void updateScale(int screenWidth);

    raylib::Vector2 screenToImage(raylib::Vector2 screen) const;
    CellRect cellRect(int col, int row) const;

    int hoveredCell(raylib::Vector2 mouse) const;

    /// Draw layers with z in [minZ, maxZ].
    void drawRange(int minZ, int maxZ) const { layers_.drawRange(minZ, maxZ); }
    void drawHover(int cellIndex) const;

    /// Layer hitboxes (row + X range) for building damage.
    const std::vector<LayerHitbox>& getHitboxes() const { return layers_.getHitboxes(); }
    const raylib::Texture* getLayerTexture(const std::string& name) const { return layers_.getTexture(name); }
    float getScale() const { return scale_; }

private:
    LayerManager layers_;
    float scale_ = 1.0f;
};
