#pragma once

#include "raylib-cpp.hpp"

/// Single- or multi-frame sprite. Single image now; spritesheet-ready.
class Sprite {
public:
    explicit Sprite(const std::string& path);
    ~Sprite() = default;

    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

    // -- GPU init (call after Window created) --
    void loadTexture();

    // -- dims --
    int width()  const { return m_frameW; }
    int height() const { return m_frameH; }

    // -- draw (screen coords) --
    void draw(int x, int y) const;
    void drawCentered(int cx, int cy) const;
    /// Scale to fit inside bw×bh while preserving aspect ratio, centered.
    void drawFitted(int bx, int by, int bw, int bh) const;

    // -- animation (spritesheet prep) --
    void setFrameCount(int count);
    int  frameCount() const { return m_frameCount; }
    void setFrame(int frame);
    int  frame() const { return m_frame; }

private:
    raylib::Image   m_image;
    raylib::Texture m_texture;
    bool m_loaded = false;

    int m_frameW      = 0;
    int m_frameH      = 0;
    int m_frameCount  = 1;
    int m_frame       = 0;
};
