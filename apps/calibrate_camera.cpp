// Interactive camera calibration with a checkerboard.
// Controls:  SPACE = capture view   c = calibrate and save   q/ESC = quit
// Run from the repo root:
//   .\build\Release\calibrate_camera.exe [config/app.yaml]

#include "aruco_pose/calibration.hpp"
#include "aruco_pose/config.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    const std::string configPath = (argc > 1) ? argv[1] : "config/app.yaml";

    try {
        const aruco_pose::AppConfig config = aruco_pose::loadConfig(configPath);
        // TODO (Phase 2): create a Calibrator with
        //       cv::Size(config.boardCols, config.boardRows) and config.squareSizeM.

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

        std::cout << "SPACE = capture, c = calibrate, q/ESC = quit\n";

        cv::Mat frame;
        while (true) {
            cap >> frame;
            if (frame.empty()) {
                break;
            }

            // TODO: show how many views are captured (cv::putText).

            cv::imshow("Calibration", frame);
            const int key = cv::waitKey(1);

            if (key == 'q' || key == 27) {
                break;
            }
            if (key == ' ') {
                // TODO: calibrator.addFrame(frame). Print whether the board was found.
                // Tip: draw the found corners (cv::drawChessboardCorners) so you can
                // see bad detections. Draw on a copy, not the frame you pass in.
            }
            if (key == 'c') {
                // TODO: rms = calibrator.calibrate(), print it, then saveIntrinsics.
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
