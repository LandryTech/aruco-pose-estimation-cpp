#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace aruco_pose {

// The camera's intrinsic parameters. Output of calibration, input to pose solving.
struct CameraIntrinsics {
    cv::Mat cameraMatrix;  // 3x3: fx, fy, cx, cy
    cv::Mat distCoeffs;    // k1, k2, p1, p2, k3
    cv::Size imageSize;    // resolution these values are valid for
};

void saveIntrinsics(const std::string& path, const CameraIntrinsics& intrinsics, double rmsError);
CameraIntrinsics loadIntrinsics(const std::string& path);

// Collects checkerboard views, then solves for the camera intrinsics.
class Calibrator {
public:
    Calibrator(cv::Size patternSize, double squareSizeM);

    // Finds the board in `frame`. If found, stores its corners and returns true.
    bool addFrame(const cv::Mat& frame);

    // Runs calibration on all stored views. Returns RMS reprojection error in pixels.
    double calibrate();

    int numFrames() const;
    const CameraIntrinsics& intrinsics() const;

    // Last detected corners, useful for drawing feedback while capturing.
    const std::vector<cv::Point2f>& lastCorners() const;

private:
    cv::Size patternSize_;
    double squareSizeM_;
    cv::Size imageSize_;

    std::vector<cv::Point3f> boardPoints_;                // one 3D grid, reused for every view
    std::vector<std::vector<cv::Point2f>> imagePoints_;   // detected corners, one set per view
    std::vector<cv::Point2f> lastCorners_;

    CameraIntrinsics intrinsics_;
};

}  // namespace aruco_pose
