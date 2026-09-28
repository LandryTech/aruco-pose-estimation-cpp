#include "aruco_pose/calibration.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>

namespace aruco_pose {

void saveIntrinsics(const std::string& path, const CameraIntrinsics& intrinsics, double rmsError) {
    // TODO: open cv::FileStorage in WRITE mode and write camera_matrix,
    // dist_coeffs, image_width, image_height, and rms_error.
    (void)path;
    (void)intrinsics;
    (void)rmsError;
}

CameraIntrinsics loadIntrinsics(const std::string& path) {
    CameraIntrinsics intrinsics;
    // TODO: open in READ mode, throw if missing, read back what saveIntrinsics wrote.
    (void)path;
    return intrinsics;
}

Calibrator::Calibrator(cv::Size patternSize, double squareSizeM)
    : patternSize_(patternSize), squareSizeM_(squareSizeM) {
    // TODO: fill boardPoints_ with the 3D position of every inner corner.
    // The board is flat, so z = 0. Loop rows, then cols, spacing by squareSizeM_.
    // Order must match the order findChessboardCorners returns corners in.
}

bool Calibrator::addFrame(const cv::Mat& frame) {
    // TODO:
    // 1. Record imageSize_ from the frame.
    // 2. Convert to grayscale (cv::cvtColor).
    // 3. cv::findChessboardCorners. If not found, return false.
    // 4. Refine with cv::cornerSubPix. Look up what the window size and
    //    termination criteria mean.
    // 5. Save corners to lastCorners_ and push them onto imagePoints_.
    (void)frame;
    return false;
}

double Calibrator::calibrate() {
    // TODO:
    // 1. Throw if you have too few views (aim for 15+).
    // 2. Build objectPoints: one copy of boardPoints_ per stored view.
    // 3. Call cv::calibrateCamera. It returns the RMS reprojection error.
    // 4. Store results in intrinsics_.
    return -1.0;
}

int Calibrator::numFrames() const {
    return static_cast<int>(imagePoints_.size());
}

const CameraIntrinsics& Calibrator::intrinsics() const {
    return intrinsics_;
}

const std::vector<cv::Point2f>& Calibrator::lastCorners() const {
    return lastCorners_;
}

}  // namespace aruco_pose
