#include "aruco_pose/transforms.hpp"

#include <opencv2/calib3d.hpp>
#include <cmath>

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
    // rvec is axis-angle: the direction is the rotation axis, the length is the angle (rad).
    // Rodrigues turns it into a 3x3 rotation matrix R (marker frame -> camera frame).
    cv::Matx33d R;
    cv::Rodrigues(rvec, R);

    // A marker facing the camera has its z axis pointing back at the camera, so R is a
    // 180 deg flip about x and roll would read +/-180 (and jump sign with noise).
    // Multiplying by Rx(180) = diag(1, -1, -1) measures angles from "facing the camera,
    // upright" instead, so that pose reads (0, 0, 0). It just negates columns 1 and 2.
    R = R * cv::Matx33d(1, 0, 0, 0, -1, 0, 0, 0, -1);

    // Convention: Z-Y-X, i.e. R = Rz(yaw) * Ry(pitch) * Rx(roll), angles about the camera axes.
    // Expanding that product and reading off entries gives the formulas below.
    // Gimbal lock: at pitch = +/-90 deg, roll and yaw spin the same axis and can't be separated.
    // Using atan2 on the (0,0)/(1,0) entries keeps pitch well-defined right up to that point.
    const double roll = std::atan2(R(2, 1), R(2, 2));
    const double pitch = std::atan2(-R(2, 0), std::sqrt(R(0, 0) * R(0, 0) + R(1, 0) * R(1, 0)));
    const double yaw = std::atan2(R(1, 0), R(0, 0));

    constexpr double kRadToDeg = 180.0 / CV_PI;
    return {roll * kRadToDeg, pitch * kRadToDeg, yaw * kRadToDeg};
}

}  // namespace aruco_pose
