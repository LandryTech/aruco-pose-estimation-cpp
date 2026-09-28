#include "aruco_pose/transforms.hpp"

#include <opencv2/calib3d.hpp>

namespace aruco_pose {

cv::Matx44d toTransform(const cv::Vec3d& rvec, const cv::Vec3d& tvec) {
    // TODO: convert rvec to a 3x3 rotation matrix with cv::Rodrigues.
    // Place R in the top-left 3x3, t in the right column, and [0 0 0 1] on the bottom.
    (void)rvec;
    (void)tvec;
    return cv::Matx44d::eye();
}

cv::Matx44d invertTransform(const cv::Matx44d& T) {
    // TODO: for a rigid transform, the inverse is R^T and -R^T * t.
    // Work out why on paper before coding it.
    (void)T;
    return cv::Matx44d::eye();
}

cv::Matx44d relativeTransform(const cv::Matx44d& T_cam_a, const cv::Matx44d& T_cam_b) {
    // TODO: T_a_b = inverse(T_cam_a) * T_cam_b
    (void)T_cam_a;
    (void)T_cam_b;
    return cv::Matx44d::eye();
}

cv::Vec3d translationOf(const cv::Matx44d& T) {
    // TODO: return the top 3 values of the last column.
    (void)T;
    return {};
}

cv::Vec3d rotationToEulerDeg(const cv::Vec3d& rvec) {
    // TODO: rvec -> rotation matrix (cv::Rodrigues) -> roll, pitch, yaw.
    // Pick one Euler convention, document it, and watch for gimbal lock.
    (void)rvec;
    return {};
}

}  // namespace aruco_pose
