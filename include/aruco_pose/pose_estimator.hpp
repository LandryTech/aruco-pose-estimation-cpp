#pragma once

#include "aruco_pose/calibration.hpp"
#include "aruco_pose/marker_detector.hpp"

#include <opencv2/core.hpp>
#include <optional>
#include <vector>

namespace aruco_pose {

// Pose of one marker in the camera frame.
struct MarkerPose {
    int id = -1;
    cv::Vec3d rvec;  // rotation, axis-angle (radians)
    cv::Vec3d tvec;  // translation, meters (marker center in camera frame)
};

// Solves a marker's 6 DoF pose from its 4 image corners with cv::solvePnP.
class PoseEstimator {
public:
    PoseEstimator(const CameraIntrinsics& intrinsics, double markerLengthM);

    // Returns nothing if solvePnP fails.
    std::optional<MarkerPose> estimate(const MarkerDetection& detection) const;

    const CameraIntrinsics& intrinsics() const;
    double markerLength() const;

private:
    CameraIntrinsics intrinsics_;
    double markerLengthM_;
    std::vector<cv::Point3f> markerPoints_;  // 4 corners in the marker's own frame
};

}  // namespace aruco_pose
