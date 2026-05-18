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
    int getWidth()  const { return frameW_; }
    int getHeight() const { return frameH_; }

    // -- draw (screen coords) --
    void draw(int x, int y) const;
    void drawCentered(int cx, int cy) const;
    /// Scale to fit inside bw×bh while preserving aspect ratio, centered.
    void drawFitted(int bx, int by, int bw, int bh) const;

    // -- animation (spritesheet prep) --
    void setFrameCount(int count);
    int  getFrameCount() const { return frameCount_; }
    void setFrame(int frame);
    int  getFrame() const { return frame_; }

private:
    raylib::Image   image_;
    raylib::Texture texture_;
    bool loaded_ = false;

    int frameW_      = 0;
    int frameH_      = 0;
    int frameCount_  = 1;
    int frame_       = 0;
};
