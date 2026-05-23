#include "layer.hpp"

#include <algorithm>
#include <cmath>
#include <dirent.h>
#include <sys/stat.h>

// ------------------------------------------------------------------
// Filename parsing
// ------------------------------------------------------------------
bool LayerManager::parseFilename(const std::string& filename,
                                 std::string& outName,
                                 int& outZ,
                                 LayerFlags& outFlags)
{
    constexpr const char* kPrefix = "layer_";
    constexpr const char* kSuffix = ".png";

    if (filename.size() < 10) return false;
    if (filename.compare(0, 6, kPrefix) != 0) return false;
    if (filename.compare(filename.size() - 4, 4, kSuffix) != 0) return false;

    std::string body = filename.substr(6, filename.size() - 10);

    std::vector<std::string> parts;
    {
        size_t start = 0;
        for (size_t i = 0; i <= body.size(); ++i) {
            if (i == body.size() || body[i] == '_') {
                parts.push_back(body.substr(start, i - start));
                start = i + 1;
            }
        }
    }
    if (parts.size() < 2) return false;

    const std::string& last = parts.back();
    const bool hasProps = (last == "x" || last == "translucent");

    outFlags = LayerFlags::None;
    if (hasProps) {
        if (last == "x")
            ;
        else if (last == "translucent")
            outFlags = LayerFlags::Translucent;
    }

    int zPartIdx = hasProps ? static_cast<int>(parts.size()) - 2
                            : static_cast<int>(parts.size()) - 1;
    if (zPartIdx < 0) return false;
    try {
        outZ = std::stoi(parts[zPartIdx]);
    } catch (...) {
        return false;
    }

    outName.clear();
    for (int i = 0; i < zPartIdx; ++i) {
        if (i > 0) outName += '_';
        outName += parts[i];
    }

    return true;
}

// ------------------------------------------------------------------
// Hitbox row assignment
// ------------------------------------------------------------------
static int assignedRowMask(const std::string& name) {
    if (name == "building_1") return (1<<0)|(1<<1)|(1<<2);
    if (name == "stone_front") return (1<<3);
    if (name == "building_4") return (1<<4);
    if (name == "building_5") return (1<<4);
    return 0;
}

// ------------------------------------------------------------------
// Alpha X range
// ------------------------------------------------------------------
static void findHitboxX(const raylib::Image& img, float& outX1, float& outX2) {
    outX1 = 1e9f; outX2 = 0.0f;
    const int w = img.GetWidth(), h = img.GetHeight();
    auto* pixels = img.LoadColors();
    if (!pixels) { outX1 = 0.0f; return; }
    for (int y = 0; y < h; ++y) {
        int left = -1, right = -1;
        for (int x = 0; x < w; ++x) if (pixels[y*w+x].a >= 128) { left = x; break; }
        for (int x = w-1; x >= 0; --x) if (pixels[y*w+x].a >= 128) { right = x; break; }
        if (left >= 0 && right >= 0) {
            if (left < outX1) outX1 = float(left);
            if (right+1 > outX2) outX2 = float(right+1);
        }
    }
    MemFree(pixels);
    if (outX1 > outX2) outX1 = outX2 = 0.0f;
}

// ------------------------------------------------------------------
// LayerManager
// ------------------------------------------------------------------
LayerManager::~LayerManager() {
    for (auto& layer : layers_) {
        layer.dayTex.Unload();
        layer.nightTex.Unload();
    }
}

void LayerManager::load(const std::string& assetDir) {
    layers_.clear(); hitboxes_.clear();
    imageW_ = 0.0f; imageH_ = 0.0f;

    DIR* dir = opendir(assetDir.c_str());
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        const std::string name(entry->d_name);
        if (name.size() < 10) continue;
        if (name.compare(0, 6, "layer_") != 0) continue;
        if (name.compare(name.size()-4, 4, ".png") != 0) continue;

        struct stat st;
        std::string fullPath = assetDir + "/" + name;
        if (stat(fullPath.c_str(), &st) != 0 || !S_ISREG(st.st_mode)) continue;

        std::string layerName;
        int zIndex = 0;
        LayerFlags flags = LayerFlags::None;
        if (!parseFilename(name, layerName, zIndex, flags)) continue;

        if (layers_.empty()) {
            raylib::Image probe(fullPath);
            imageW_ = float(probe.GetWidth());
            imageH_ = float(probe.GetHeight());
        }

        Layer layer;
        layer.name      = layerName;
        layer.dayPath   = fullPath;
        layer.nightPath = assetDir + "/night/" + name;
        layer.zIndex    = zIndex;
        layer.flags     = flags;

        // Hitboxes
        int mask = assignedRowMask(layerName);
        if (mask) {
            raylib::Image probe(fullPath);
            float hx1, hx2;
            findHitboxX(probe, hx1, hx2);
            for (int r = 0; r < 5; ++r)
                if (mask & (1 << r))
                    hitboxes_.push_back({r, hx1, hx2});
        }

        layers_.push_back(std::move(layer));
    }
    closedir(dir);
    std::sort(layers_.begin(), layers_.end(),
              [](const Layer& a, const Layer& b) { return a.zIndex < b.zIndex; });
}

void LayerManager::upload() {
    for (auto& layer : layers_) {
        // Day texture
        raylib::Image dayImg(layer.dayPath);
        layer.dayTex.Load(dayImg);
        layer.dayTex.SetFilter(TEXTURE_FILTER_POINT);
        dayImg.Unload();

        // Night texture (if exists)
        struct stat st;
        if (stat(layer.nightPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
            raylib::Image nightImg(layer.nightPath);
            layer.nightTex.Load(nightImg);
            layer.nightTex.SetFilter(TEXTURE_FILTER_POINT);
            nightImg.Unload();
        }
    }
}

void LayerManager::drawRange(int minZ, int maxZ) const {
    const float sw = float(GetScreenWidth()), sh = float(GetScreenHeight());
    const Rectangle srcFull{0,0,imageW_,imageH_};
    const Rectangle dstFull{0,0,sw,sh};

    for (const auto& layer : layers_) {
        if (layer.zIndex < minZ || layer.zIndex > maxZ) continue;

        const raylib::Texture* tex = nightMode_ && layer.nightTex.IsValid()
            ? &layer.nightTex : &layer.dayTex;
        if (!tex->IsValid()) continue;

        raylib::Color tint = raylib::Color::White();
        if (static_cast<int>(layer.flags) & static_cast<int>(LayerFlags::Translucent))
            tint.a = 102;

        tex->Draw(srcFull, dstFull, {}, 0.0f, tint);
    }
}

const raylib::Texture* LayerManager::getTexture(const std::string& layerName) const {
    for (const auto& layer : layers_)
        if (layer.name == layerName) {
            const auto* tex = nightMode_ && layer.nightTex.IsValid()
                ? &layer.nightTex : &layer.dayTex;
            return tex->IsValid() ? tex : nullptr;
        }
    return nullptr;
}
