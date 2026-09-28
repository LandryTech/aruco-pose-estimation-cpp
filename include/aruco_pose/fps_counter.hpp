#pragma once

#include <chrono>

namespace aruco_pose {

// Measures frames per second, smoothed over a short window.
class FpsCounter {
public:
    // Call once per frame.
    void tick();
    double fps() const;

private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point windowStart_ = Clock::now();
    int framesInWindow_ = 0;
    double fps_ = 0.0;
};

}  // namespace aruco_pose
