#include "sprite.hpp"

Sprite::Sprite(const std::string& path) {
    image_.Load(path);
    frameW_ = image_.GetWidth();
    frameH_ = image_.GetHeight();
}

void Sprite::loadTexture() {
    texture_.Load(image_);
    image_.Unload();
    texture_.SetFilter(TEXTURE_FILTER_POINT);
    loaded_ = true;
}

void Sprite::draw(int x, int y) const {
    if (!loaded_) return;
    const Rectangle src{
        0.0f, 0.0f,
        static_cast<float>(frameW_),
        static_cast<float>(frameH_)
    };
    const Rectangle dst{
        static_cast<float>(x), static_cast<float>(y),
        static_cast<float>(frameW_),
        static_cast<float>(frameH_)
    };
    texture_.Draw(src, dst, {}, 0.0f, raylib::Color::White());
}

void Sprite::drawCentered(int cx, int cy) const {
    draw(cx - frameW_ / 2, cy - frameH_ / 2);
}

void Sprite::drawFitted(int bx, int by, int bw, int bh) const {
    if (!loaded_) return;
    const float spriteAspect =
        static_cast<float>(frameW_) / static_cast<float>(frameH_);
    const float boxAspect =
        static_cast<float>(bw) / static_cast<float>(bh);

    float sw, sh;
    if (spriteAspect > boxAspect) {
        // Fit to width
        sw = static_cast<float>(bw);
        sh = sw / spriteAspect;
    } else {
        // Fit to height
        sh = static_cast<float>(bh);
        sw = sh * spriteAspect;
    }

    const float ox = static_cast<float>(bx) + (static_cast<float>(bw) - sw) / 2.0f;
    const float oy = static_cast<float>(by) + (static_cast<float>(bh) - sh) / 2.0f;

    const Rectangle src{
        0.0f, 0.0f,
        static_cast<float>(frameW_),
        static_cast<float>(frameH_)
    };
    const Rectangle dst{ox, oy, sw, sh};
    texture_.Draw(src, dst, {}, 0.0f, raylib::Color::White());
}
