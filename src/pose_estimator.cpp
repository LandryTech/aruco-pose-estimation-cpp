#include "aruco_pose/pose_estimator.hpp"

#include <opencv2/calib3d.hpp>

namespace aruco_pose {

PoseEstimator::PoseEstimator(const CameraIntrinsics& intrinsics, double markerLengthM)
    : intrinsics_(intrinsics), markerLengthM_(markerLengthM) {
    // TODO: fill markerPoints_ with the 4 marker corners in the marker's frame.
    // Center at the origin, z = 0, half-length = markerLengthM_ / 2.
    // Order MUST match the detector: top-left, top-right, bottom-right, bottom-left.
    // Think about which way +x and +y point in the marker frame.
}

std::optional<MarkerPose> PoseEstimator::estimate(const MarkerDetection& detection) const {
    // TODO:
    // 1. Call cv::solvePnP with markerPoints_, detection.corners,
    //    the camera matrix, and dist coeffs.
    //    Use the flag cv::SOLVEPNP_IPPE_SQUARE (read why it fits square markers).
    // 2. If it returns false, return std::nullopt.
    // 3. Otherwise return a MarkerPose with the id, rvec, and tvec.
    (void)detection;
    return std::nullopt;
}

const CameraIntrinsics& PoseEstimator::intrinsics() const {
    return intrinsics_;
}

double PoseEstimator::markerLength() const {
    return markerLengthM_;
}

}  // namespace aruco_pose
