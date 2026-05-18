#include "sprite.hpp"

Sprite::Sprite(const std::string& path) {
    m_image.Load(path);
    m_frameW = m_image.GetWidth();
    m_frameH = m_image.GetHeight();
}

void Sprite::loadTexture() {
    m_texture.Load(m_image);
    m_image.Unload();
    m_texture.SetFilter(TEXTURE_FILTER_POINT);
    m_loaded = true;
}

void Sprite::draw(int x, int y) const {
    if (!m_loaded) return;
    const Rectangle src{
        static_cast<float>(m_frame * m_frameW), 0.0f,
        static_cast<float>(m_frameW),
        static_cast<float>(m_frameH)
    };
    const Rectangle dst{
        static_cast<float>(x), static_cast<float>(y),
        static_cast<float>(m_frameW),
        static_cast<float>(m_frameH)
    };
    m_texture.Draw(src, dst, {}, 0.0f, raylib::Color::White());
}

void Sprite::drawCentered(int cx, int cy) const {
    draw(cx - m_frameW / 2, cy - m_frameH / 2);
}

void Sprite::drawFitted(int bx, int by, int bw, int bh) const {
    if (!m_loaded) return;
    const float spriteAspect =
        static_cast<float>(m_frameW) / static_cast<float>(m_frameH);
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
        static_cast<float>(m_frame * m_frameW), 0.0f,
        static_cast<float>(m_frameW),
        static_cast<float>(m_frameH)
    };
    const Rectangle dst{ox, oy, sw, sh};
    m_texture.Draw(src, dst, {}, 0.0f, raylib::Color::White());
}

void Sprite::setFrameCount(int count) {
    m_frameCount = count > 0 ? count : 1;
    m_frame = 0;
}

void Sprite::setFrame(int frame) {
    m_frame = (frame >= 0 && frame < m_frameCount) ? frame : 0;
}
