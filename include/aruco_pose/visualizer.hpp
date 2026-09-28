#pragma once

#include "aruco_pose/marker_detector.hpp"
#include "aruco_pose/pose_estimator.hpp"

#include <opencv2/core.hpp>
#include <vector>

namespace aruco_pose {

// All drawing lives here so the math classes stay free of display code.
class Visualizer {
public:
    Visualizer(const CameraIntrinsics& intrinsics, double axisLengthM);

    void drawDetections(cv::Mat& frame, const std::vector<MarkerDetection>& detections) const;
    void drawAxes(cv::Mat& frame, const MarkerPose& pose) const;
    void drawPoseText(cv::Mat& frame, const MarkerPose& pose, cv::Point origin) const;
    void drawFps(cv::Mat& frame, double fps) const;

private:
    CameraIntrinsics intrinsics_;
    double axisLengthM_;
};

}  // namespace aruco_pose
