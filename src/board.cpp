#include "board.hpp"

#include <cmath>

namespace {
    constexpr char kDefaultCell = ' ';
}

Board::Board(const std::string& bgPath) {
    raylib::Image img(bgPath);
    m_imageW = static_cast<float>(img.GetWidth());
    m_imageH = static_cast<float>(img.GetHeight());

    // Init cell grid (reference Board pattern)
    m_data = std::vector<std::vector<char>>(
        m_numRows, std::vector<char>(m_numCols, kDefaultCell));

    // Boost bg vibrancy (CPU-side, safe before Window)
    img.ColorContrast(1.3f);
    img.ColorBrightness(8);

    // Keep processed image for later GPU upload
    m_processedImage = img;
    // Don't unload img — ownership transferred to m_processedImage
}

void Board::loadTexture() {
    m_background.Load(m_processedImage);  // GPU upload (needs GL context)
    m_processedImage.Unload();            // CPU copy no longer needed
    m_background.SetFilter(TEXTURE_FILTER_POINT);
}

void Board::updateScale(int screenWidth) {
    m_scale = static_cast<float>(screenWidth) / m_imageW;
}

raylib::Vector2 Board::screenToImage(Vector2 screen) const {
    return {screen.x / m_scale, screen.y / m_scale};
}

CellRect Board::cellRect(int col, int row) const {
    return {
        static_cast<int>(std::round((kOriginX + static_cast<float>(col) * kCellW) * m_scale)),
        static_cast<int>(std::round((kOriginY + static_cast<float>(row) * kCellH) * m_scale)),
        static_cast<int>(std::round(kCellW * m_scale)),
        static_cast<int>(std::round(kCellH * m_scale))
    };
}

int Board::hoveredCell(Vector2 mouse) const {
    const raylib::Vector2 img = screenToImage(mouse);
    if (img.x < kOriginX || img.y < kOriginY) return -1;

    const int col = static_cast<int>((img.x - kOriginX) / kCellW);
    const int row = static_cast<int>((img.y - kOriginY) / kCellH);
    if (col < 0 || col >= kCols || row < 0 || row >= kRows) return -1;

    return col + row * kCols;
}

bool Board::isValid(int row, int col) const {
    return row >= 0 && row < m_numRows && col >= 0 && col < m_numCols;
}

char Board::getCell(int row, int col) const {
    return isValid(row, col) ? m_data[row][col] : kDefaultCell;
}

void Board::setCell(int row, int col, char val) {
    if (isValid(row, col)) {
        m_data[row][col] = val;
    }
}

void Board::draw() const {
    const Rectangle src{0.0f, 0.0f, m_imageW, m_imageH};
    const Rectangle dst{0.0f, 0.0f,
                        static_cast<float>(GetScreenWidth()),
                        static_cast<float>(GetScreenHeight())};
    m_background.Draw(src, dst, {}, 0.0f, raylib::Color::White());
}

void Board::drawHover(int cellIndex) const {
    if (cellIndex < 0) return;

    const int col = cellIndex % kCols;
    const int row = cellIndex / kCols;
    const auto [x, y, w, h] = cellRect(col, row);

    DrawRectangle(x, y, w, h, {255, 220, 100, 55});
}
