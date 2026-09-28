#pragma once

#include <opencv2/core.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <vector>

namespace aruco_pose {

// One detected marker: its ID and its 4 image corners.
// Corner order from OpenCV: top-left, top-right, bottom-right, bottom-left.
struct MarkerDetection {
    int id = -1;
    std::vector<cv::Point2f> corners;
};

// Wraps cv::aruco::ArucoDetector (OpenCV 4.7+ API).
class MarkerDetector {
public:
    explicit MarkerDetector(cv::aruco::PredefinedDictionaryType dictionary = cv::aruco::DICT_6X6_250);

    std::vector<MarkerDetection> detect(const cv::Mat& frame) const;

private:
    cv::aruco::ArucoDetector detector_;
};

}  // namespace aruco_pose
