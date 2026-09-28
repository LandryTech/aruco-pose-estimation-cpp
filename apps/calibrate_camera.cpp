// Interactive camera calibration with a checkerboard.
// Controls:  SPACE = capture view   c = calibrate and save   q/ESC = quit
// Run from the repo root:
//   .\build\Release\calibrate_camera.exe [config/app.yaml]

#include "aruco_pose/calibration.hpp"
#include "aruco_pose/config.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>  // OpenCV 5: drawChessboardCorners lives here
#include <opencv2/videoio.hpp>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    const std::string configPath = (argc > 1) ? argv[1] : "config/app.yaml";

    try {
        const aruco_pose::AppConfig config = aruco_pose::loadConfig(configPath);
        const cv::Size patternSize(config.boardCols, config.boardRows);
        aruco_pose::Calibrator calibrator(patternSize, config.squareSizeM);

        cv::VideoCapture cap(config.cameraIndex, cv::CAP_DSHOW);
        if (!cap.isOpened()) {
            std::cerr << "Error: could not open camera " << config.cameraIndex << ".\n";
            return 1;
        }

        // Must match pose_app: intrinsics are only valid at the resolution they were measured at.
        cap.set(cv::CAP_PROP_FRAME_WIDTH, config.frameWidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.frameHeight);
        std::cout << "Requested " << config.frameWidth << "x" << config.frameHeight
                  << ", got " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << "x"
                  << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << "\n";

        std::cout << "Board: " << patternSize.width << "x" << patternSize.height
                  << " inner corners, square " << config.squareSizeM * 1000 << " mm\n"
                  << "SPACE = capture, c = calibrate, q/ESC = quit\n";

        cv::Mat frame;
        while (true) {
            cap >> frame;
            if (frame.empty()) {
                break;
            }

            // Draw on a copy so the frame passed to addFrame stays clean.
            cv::Mat display = frame.clone();
            cv::putText(display, "Views: " + std::to_string(calibrator.numFrames()), cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
            cv::imshow("Calibration", display);

            const int key = cv::waitKey(1);
            if (key == 'q' || key == 27) {
                break;
            }
            if (key == ' ') {
                if (calibrator.addFrame(frame)) {
                    std::cout << "View " << calibrator.numFrames() << " captured.\n";
                    // Show the corners that were stored, so bad detections are easy to spot.
                    cv::Mat captured = frame.clone();
                    cv::drawChessboardCorners(captured, patternSize, calibrator.lastCorners(), true);
                    cv::imshow("Last capture", captured);
                } else {
                    std::cout << "Board not found. Make sure the whole board is visible and in focus.\n";
                }
            }
            if (key == 'c') {
                std::cout << "Calibrating with " << calibrator.numFrames() << " views...\n";
                const double rms = calibrator.calibrate();
                const aruco_pose::CameraIntrinsics& in = calibrator.intrinsics();

                std::cout << "RMS reprojection error: " << rms << " px"
                          << (rms < 0.5 ? " (great)" : rms < 1.0 ? " (ok)" : " (poor, recapture)") << "\n"
                          << "Camera matrix:\n" << in.cameraMatrix << "\n"
                          << "Distortion (k1 k2 p1 p2 k3): " << in.distCoeffs << "\n";

                aruco_pose::saveIntrinsics(config.intrinsicsFile, in, rms);
                std::cout << "Saved to " << config.intrinsicsFile << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
