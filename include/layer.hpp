#pragma once

#include <string>
#include <vector>

#include "raylib-cpp.hpp"

/// Layer properties parsed from filename: layer_[name]_[z]_[props].png
enum class LayerFlags {
    None        = 0,
    Translucent = 1 << 0,  ///< rendered at 40% opacity
};

/// One loaded layer with textures and metadata.
struct Layer {
    std::string name;
    std::string dayPath;
    std::string nightPath;
    int         zIndex = 0;
    LayerFlags  flags  = LayerFlags::None;
    raylib::Texture dayTex;
    raylib::Texture nightTex;
};

/// A layer hitbox: row + X range in image coords. Enemies passing through
/// this zone deal damage to the main building.
struct LayerHitbox {
    int   row;
    float x1;  ///< leftmost alpha pixel (image coords)
    float x2;  ///< rightmost alpha pixel (image coords)
};

/// Manages multi-layer background: loads layer_*.png, sorts by z-index,
/// renders stacked, provides row-blocking info for enemy movement.
class LayerManager {
public:
    LayerManager() = default;
    ~LayerManager();

    LayerManager(const LayerManager&) = delete;
    LayerManager& operator=(const LayerManager&) = delete;
    void load(const std::string& assetDir);

    /// Upload all textures to GPU (call after Window created).
    void upload();

    /// Toggle night mode (no reload, pre-loaded textures).
    void setNightMode(bool n) { nightMode_ = n; }
    void setScale(float s) { scale_ = s; }

    /// Draw layers with zIndex in [minZ, maxZ] (inclusive).
    void drawRange(int minZ, int maxZ) const;

    /// Hitbox data derived from collidable layer alpha.
    const std::vector<LayerHitbox>& getHitboxes() const { return hitboxes_; }

    /// Get texture for a named layer (e.g. "building_1"), null if not found.
    const raylib::Texture* getTexture(const std::string& layerName) const;

    float getWidth()  const { return imageW_; }
    float getHeight() const { return imageH_; }
    float getAspectRatio() const { return (imageH_ > 0.0f) ? imageW_ / imageH_ : 1.0f; }

private:
    std::vector<Layer> layers_;
    std::vector<LayerHitbox> hitboxes_;
    bool nightMode_ = false;
    float imageW_ = 0.0f;
    float imageH_ = 0.0f;
    float scale_  = 1.0f;

    static bool parseFilename(const std::string& filename,
                              std::string& outName, int& outZ, LayerFlags& outFlags);
};
