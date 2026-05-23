#pragma once

#include <cmath>

/// Damped harmonic oscillator — iOS-style spring with overshoot + settle.
/// a = -k*disp - d*vel  →  semi-implicit Euler.
class SpringAnim {
public:
    SpringAnim() = default;
    SpringAnim(float stiffness, float damping)
        : stiffness_(stiffness), damping_(damping) {}

    /// Advance physics by dt.  Call once per frame.
    /// a = -k*disp - d*vel  →  semi-implicit Euler  →  iOS-style bounce.
    void update(float dt) {
        if (isSettled()) { snapTo(target_); return; }
        const float disp = value_ - target_;
        velocity_ += (-stiffness_ * disp - damping_ * velocity_) * dt;
        value_ += velocity_ * dt;
        if (isSettled()) snapTo(target_);
    }

    void setTarget(float t) { target_ = t; }
    float getTarget() const { return target_; }

    /// Jump to value immediately (no animation).
    void snapTo(float v) { value_ = v; velocity_ = 0.0f; }

    float getValue()             const { return value_; }
    float getVelocity()          const { return velocity_; }
    bool  isSettled(float eps = 0.001f) const {
        return std::abs(value_ - target_) < eps && std::abs(velocity_) < eps;
    }

    void setStiffness(float k) { stiffness_ = k; }
    void setDamping(float d)   { damping_ = d; }

private:
    float value_    = 0.0f;
    float velocity_ = 0.0f;
    float target_   = 0.0f;
    float stiffness_ = 200.0f;
    float damping_   = 15.0f;
};
