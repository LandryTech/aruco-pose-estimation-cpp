#include "aruco_pose/fps_counter.hpp"

namespace aruco_pose {

namespace {
// How often the FPS number updates. Short enough to react, long enough not to flicker.
constexpr double kWindowSeconds = 0.5;
}  // namespace

void FpsCounter::tick() {
    ++framesInWindow_;

    const Clock::time_point now = Clock::now();
    const double elapsed = std::chrono::duration<double>(now - windowStart_).count();

    if (elapsed >= kWindowSeconds) {
        fps_ = framesInWindow_ / elapsed;
        framesInWindow_ = 0;
        windowStart_ = now;
    }
}

double FpsCounter::fps() const {
    return fps_;
}

}  // namespace aruco_pose
