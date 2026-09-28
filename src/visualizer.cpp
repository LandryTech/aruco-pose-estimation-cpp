#include "aruco_pose/visualizer.hpp"
#include "aruco_pose/transforms.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <iomanip>
#include <sstream>

namespace aruco_pose {

Visualizer::Visualizer(const CameraIntrinsics& intrinsics, double axisLengthM)
    : intrinsics_(intrinsics), axisLengthM_(axisLengthM) {}

void Visualizer::drawDetections(cv::Mat& frame, const std::vector<MarkerDetection>& detections) const {
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
    for (const MarkerDetection& d : detections) {
        corners.push_back(d.corners);
        ids.push_back(d.id);
    }
    // Green outline, a small red square on corner 0 (top-left), and the ID.
    cv::aruco::drawDetectedMarkers(frame, corners, ids);

    // 2D position: the marker center in pixels (average of the 4 corners).
    // (0,0) is the top-left of the image, x grows right, y grows down.
    for (const MarkerDetection& d : detections) {
        cv::Point2f center(0.f, 0.f);
        for (const cv::Point2f& c : d.corners) {
            center += c;
        }
        center *= 0.25f;

        cv::circle(frame, center, 4, cv::Scalar(0, 0, 255), cv::FILLED);
        std::ostringstream text;
        text << std::fixed << std::setprecision(0) << "(" << center.x << ", " << center.y << ") px";
        cv::putText(frame, text.str(), center + cv::Point2f(8.f, 20.f), cv::FONT_HERSHEY_SIMPLEX,
                    0.5, cv::Scalar(0, 255, 255), 1);
    }
}

void Visualizer::drawAxes(cv::Mat& frame, const MarkerPose& pose) const {
    // Projects the marker's 3D axes into the image: x = red, y = green, z = blue.
    // If the pose is right, the axes stick to the marker as you move it. A quick visual check.
    cv::drawFrameAxes(frame, intrinsics_.cameraMatrix, intrinsics_.distCoeffs, pose.rvec, pose.tvec,
                      static_cast<float>(axisLengthM_));
}

void Visualizer::drawPoseText(cv::Mat& frame, const MarkerPose& pose, cv::Point origin) const {
    const cv::Vec3d& t = pose.tvec;
    const double distance = cv::norm(t);  // straight-line distance, camera to marker center
    const cv::Vec3d rpy = rotationToEulerDeg(pose.rvec);

    std::ostringstream line1;
    std::ostringstream line2;
    // Meters -> cm for readability.
    line1 << std::fixed << std::setprecision(1) << "ID " << pose.id << "  X " << t[0] * 100
          << "  Y " << t[1] * 100 << "  Z " << t[2] * 100 << "  dist " << distance * 100 << " cm";
    line2 << std::fixed << std::setprecision(1) << "   roll " << rpy[0] << "  pitch " << rpy[1]
          << "  yaw " << rpy[2] << " deg";

    const cv::Scalar color(255, 255, 0);
    cv::putText(frame, line1.str(), origin, cv::FONT_HERSHEY_SIMPLEX, 0.55, color, 1);
    cv::putText(frame, line2.str(), origin + cv::Point(0, 22), cv::FONT_HERSHEY_SIMPLEX, 0.55, color, 1);
}

void Visualizer::drawFps(cv::Mat& frame, double fps) const {
    std::ostringstream text;
    text << "FPS: " << std::fixed << std::setprecision(1) << fps;
    cv::putText(frame, text.str(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8,
                cv::Scalar(0, 255, 0), 2);
}

}  // namespace aruco_pose
