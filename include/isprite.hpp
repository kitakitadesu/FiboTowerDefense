#pragma once

/// Drawable interface. All renderable entities implement this.
class ISprite {
public:
    virtual void draw() const = 0;
    virtual ~ISprite() = default;
};
