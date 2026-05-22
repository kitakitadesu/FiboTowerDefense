#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "raylib-cpp.hpp"

struct FloatingText {
    raylib::Vector2 pos;
    std::string text;
    raylib::Color color;
    float life;
    float maxLife;
    raylib::Vector2 velocity;
};

/// Erase elements matching a predicate, return count removed.
template <typename T, typename Pred>
int eraseIf(std::vector<T>& vec, Pred pred) {
    auto it = std::remove_if(vec.begin(), vec.end(), pred);
    int n = static_cast<int>(vec.end() - it);
    vec.erase(it, vec.end());
    return n;
}
