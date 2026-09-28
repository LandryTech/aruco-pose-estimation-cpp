#include "aruco_pose/visualizer.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <iomanip>
#include <sstream>

namespace aruco_pose {

Visualizer::Visualizer(const CameraIntrinsics& intrinsics, double axisLengthM)
    : intrinsics_(intrinsics), axisLengthM_(axisLengthM) {}

void Visualizer::drawDetections(cv::Mat& frame, const std::vector<MarkerDetection>& detections) const {
    // TODO: split detections back into corners and ids, then call
    // cv::aruco::drawDetectedMarkers.
    (void)frame;
    (void)detections;
}

void Visualizer::drawAxes(cv::Mat& frame, const MarkerPose& pose) const {
    // TODO: cv::drawFrameAxes with the intrinsics, pose.rvec, pose.tvec, axisLengthM_.
    (void)frame;
    (void)pose;
}

void Visualizer::drawPoseText(cv::Mat& frame, const MarkerPose& pose, cv::Point origin) const {
    // TODO: format ID, X/Y/Z (cm is easier to read), distance, and roll/pitch/yaw.
    // Draw with cv::putText. Look up std::ostringstream and std::fixed for formatting.
    (void)frame;
    (void)pose;
    (void)origin;
}

void Visualizer::drawFps(cv::Mat& frame, double fps) const {
    std::ostringstream text;
    text << "FPS: " << std::fixed << std::setprecision(1) << fps;
    cv::putText(frame, text.str(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8,
                cv::Scalar(0, 255, 0), 2);
}

}  // namespace aruco_pose
