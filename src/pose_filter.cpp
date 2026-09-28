#include "aruco_pose/pose_filter.hpp"

namespace aruco_pose {

PoseFilter::PoseFilter(double alpha) : alpha_(alpha) {}

MarkerPose PoseFilter::update(const MarkerPose& measurement) {
    // TODO:
    // 1. If this marker ID has no state yet, store the measurement and return it.
    // 2. Otherwise blend translation: filtered = alpha * new + (1 - alpha) * old.
    // 3. Rotation: do NOT blend rvecs directly. Pass it through for now, or
    //    convert to quaternions and slerp. Read why averaging rvecs is wrong.
    (void)alpha_;
    return measurement;
}

void PoseFilter::reset() {
    state_.clear();
}

}  // namespace aruco_pose
