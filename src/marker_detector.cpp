#include "aruco_pose/marker_detector.hpp"

namespace aruco_pose {

namespace {

cv::aruco::DetectorParameters makeParams() {
    cv::aruco::DetectorParameters params;
    // Default corners are snapped to whole pixels. Sub-pixel refinement fits each
    // corner to the actual edge gradient, so corners are more precise and jitter less.
    // Pose accuracy in Phase 4 comes straight from these corners.
    params.cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX;
    return params;
}

}  // namespace

MarkerDetector::MarkerDetector(cv::aruco::PredefinedDictionaryType dictionary)
    : detector_(cv::aruco::getPredefinedDictionary(dictionary), makeParams()) {}

std::vector<MarkerDetection> MarkerDetector::detect(const cv::Mat& frame) const {
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
    detector_.detectMarkers(frame, corners, ids);

    // OpenCV returns two parallel lists (ids[i] belongs to corners[i]).
    // Pairing them into one struct means they can never get out of sync later.
    std::vector<MarkerDetection> detections;
    detections.reserve(ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        detections.push_back({ids[i], corners[i]});
    }
    return detections;
}

}  // namespace aruco_pose
