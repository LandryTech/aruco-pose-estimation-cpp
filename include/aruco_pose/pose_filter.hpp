#pragma once

#include "aruco_pose/pose_estimator.hpp"

#include <map>

namespace aruco_pose {

// Smooths pose jitter over time (Phase 6). Keeps separate state per marker ID.
class PoseFilter {
public:
    // alpha in (0, 1]. Higher = more responsive, lower = smoother but more lag.
    explicit PoseFilter(double alpha);

    MarkerPose update(const MarkerPose& measurement);
    void reset();

private:
    double alpha_;
    std::map<int, MarkerPose> state_;
};

}  // namespace aruco_pose
