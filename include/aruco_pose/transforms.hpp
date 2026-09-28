#pragma once

#include <opencv2/core.hpp>

namespace aruco_pose {

// Helpers for 4x4 homogeneous transforms. Used for relative pose (Phase 5).
// Naming: T_a_b means "pose of frame b expressed in frame a".

cv::Matx44d toTransform(const cv::Vec3d& rvec, const cv::Vec3d& tvec);

// Inverts a rigid transform. Hint: there's a cheaper way than a general matrix inverse.
cv::Matx44d invertTransform(const cv::Matx44d& T);

// Pose of marker B in marker A's frame, given both in the camera frame.
cv::Matx44d relativeTransform(const cv::Matx44d& T_cam_a, const cv::Matx44d& T_cam_b);

cv::Vec3d translationOf(const cv::Matx44d& T);

// Roll, pitch, yaw in degrees, for display only.
cv::Vec3d rotationToEulerDeg(const cv::Vec3d& rvec);

}  // namespace aruco_pose
