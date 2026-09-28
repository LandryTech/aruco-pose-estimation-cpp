#include "aruco_pose/calibration.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>  // OpenCV 5: findChessboardCorners lives here
#include <stdexcept>

namespace aruco_pose {

namespace {
// Fewer views than this can't pin down 9 unknowns (fx, fy, cx, cy, 5 distortion) reliably.
constexpr int kMinViews = 10;
}  // namespace

void saveIntrinsics(const std::string& path, const CameraIntrinsics& intrinsics, double rmsError) {
    cv::FileStorage fs(path, cv::FileStorage::WRITE);
    if (!fs.isOpened()) {
        throw std::runtime_error("Could not write intrinsics file: " + path);
    }
    fs << "image_width" << intrinsics.imageSize.width;
    fs << "image_height" << intrinsics.imageSize.height;
    fs << "camera_matrix" << intrinsics.cameraMatrix;
    fs << "dist_coeffs" << intrinsics.distCoeffs;
    fs << "rms_error" << rmsError;
}

CameraIntrinsics loadIntrinsics(const std::string& path) {
    cv::FileStorage fs(path, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        throw std::runtime_error("Could not open intrinsics file: " + path +
                                 " (run calibrate_camera first)");
    }

    CameraIntrinsics intrinsics;
    fs["image_width"] >> intrinsics.imageSize.width;
    fs["image_height"] >> intrinsics.imageSize.height;
    fs["camera_matrix"] >> intrinsics.cameraMatrix;
    fs["dist_coeffs"] >> intrinsics.distCoeffs;

    if (intrinsics.cameraMatrix.size() != cv::Size(3, 3) || intrinsics.distCoeffs.empty()) {
        throw std::runtime_error("Intrinsics file is missing or has a bad camera_matrix/dist_coeffs: " + path);
    }
    return intrinsics;
}

Calibrator::Calibrator(cv::Size patternSize, double squareSizeM)
    : patternSize_(patternSize), squareSizeM_(squareSizeM) {
    // The board defines its own 3D frame: origin at the first inner corner, z = 0 on the paper.
    // findChessboardCorners returns corners row by row, left to right, so we fill in the same order.
    // Real units here (meters) are what make the camera's pose solution come out in meters.
    for (int row = 0; row < patternSize_.height; ++row) {
        for (int col = 0; col < patternSize_.width; ++col) {
            boardPoints_.emplace_back(static_cast<float>(col * squareSizeM_),
                                      static_cast<float>(row * squareSizeM_), 0.f);
        }
    }
}

bool Calibrator::addFrame(const cv::Mat& frame) {
    if (imageSize_.empty()) {
        imageSize_ = frame.size();
    } else if (frame.size() != imageSize_) {
        throw std::runtime_error("Frame size changed during calibration. All views must be the same resolution.");
    }

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // ADAPTIVE_THRESH + NORMALIZE_IMAGE handle uneven lighting.
    // FAST_CHECK bails out early when there is clearly no board, so a miss is cheap.
    std::vector<cv::Point2f> corners;
    const int flags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK;
    if (!cv::findChessboardCorners(gray, patternSize_, corners, flags)) {
        return false;
    }

    // findChessboardCorners is only accurate to about a pixel. cornerSubPix moves each corner
    // to where the black/white edges actually cross, inside an 11x11 search window.
    // It stops after 30 iterations or when a corner moves less than 0.001 px.
    const cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001);
    cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);

    lastCorners_ = corners;
    imagePoints_.push_back(corners);
    return true;
}

double Calibrator::calibrate() {
    if (numFrames() < kMinViews) {
        throw std::runtime_error("Need at least " + std::to_string(kMinViews) + " views, have " +
                                 std::to_string(numFrames()) + ". Aim for 15-25.");
    }

    // Same physical board in every view, so every view shares the same 3D points.
    const std::vector<std::vector<cv::Point3f>> objectPoints(imagePoints_.size(), boardPoints_);

    // Solves for the intrinsics (shared by all views) plus one board pose per view (rvecs/tvecs),
    // minimizing the pixel distance between detected corners and where the model projects them.
    // The return value is the RMS of that distance: the main quality number.
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;
    const double rms = cv::calibrateCamera(objectPoints, imagePoints_, imageSize_,
                                           intrinsics_.cameraMatrix, intrinsics_.distCoeffs,
                                           rvecs, tvecs);
    intrinsics_.imageSize = imageSize_;
    return rms;
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
