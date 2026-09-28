#include "aruco_pose/marker_detector.hpp"

namespace aruco_pose {

MarkerDetector::MarkerDetector(cv::aruco::PredefinedDictionaryType dictionary)
    : detector_(cv::aruco::getPredefinedDictionary(dictionary),
                cv::aruco::DetectorParameters()) {
    // Optional later: tune DetectorParameters (e.g. corner refinement method).
}

std::vector<MarkerDetection> MarkerDetector::detect(const cv::Mat& frame) const {
    std::vector<MarkerDetection> detections;

    // TODO:
    // 1. Call detector_.detectMarkers(frame, corners, ids).
    //    corners is std::vector<std::vector<cv::Point2f>>, ids is std::vector<int>.
    // 2. Pair each id with its corners into a MarkerDetection.
    (void)frame;

    return detections;
}

}  // namespace aruco_pose
