#include "aruco_pose/pose_estimator.hpp"

#include <opencv2/calib3d.hpp>

namespace aruco_pose {

PoseEstimator::PoseEstimator(const CameraIntrinsics& intrinsics, double markerLengthM)
    : intrinsics_(intrinsics), markerLengthM_(markerLengthM) {
    // The marker's own frame: origin at its center, x right, y up, z out of the paper
    // (toward the camera when it faces you). These 4 points are the corners in that frame,
    // in the detector's order (TL, TR, BR, BL). SOLVEPNP_IPPE_SQUARE requires exactly this layout.
    const float h = static_cast<float>(markerLengthM_ / 2.0);
    markerPoints_ = {
        {-h,  h, 0.f},  // top-left
        { h,  h, 0.f},  // top-right
        { h, -h, 0.f},  // bottom-right
        {-h, -h, 0.f},  // bottom-left
    };
}

std::optional<MarkerPose> PoseEstimator::estimate(const MarkerDetection& detection) const {
    // PnP ("Perspective-n-Point"): given known 3D points and where they show up in the image,
    // find the rotation and translation that best line them up through the camera model.
    // IPPE_SQUARE is a closed-form solver made for exactly 4 points on a square, so it is
    // fast and doesn't need a starting guess like the general iterative solver does.
    MarkerPose pose;
    pose.id = detection.id;
    const bool ok = cv::solvePnP(markerPoints_, detection.corners, intrinsics_.cameraMatrix,
                                 intrinsics_.distCoeffs, pose.rvec, pose.tvec, false,
                                 cv::SOLVEPNP_IPPE_SQUARE);

    // A marker behind the camera (z <= 0) is physically impossible, so treat it as a failure.
    if (!ok || pose.tvec[2] <= 0.0) {
        return std::nullopt;
    }
    return pose;
}

const CameraIntrinsics& PoseEstimator::intrinsics() const {
    return intrinsics_;
}

double PoseEstimator::markerLength() const {
    return markerLengthM_;
}

}  // namespace aruco_pose
