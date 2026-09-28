// Live ArUco 6 DoF pose estimation.
// Run from the repo root so the default config path works:
//   .\build\Release\pose_app.exe [config/app.yaml]

#include "aruco_pose/calibration.hpp"
#include "aruco_pose/config.hpp"
#include "aruco_pose/fps_counter.hpp"
#include "aruco_pose/marker_detector.hpp"
#include "aruco_pose/pose_estimator.hpp"
#include "aruco_pose/pose_filter.hpp"
#include "aruco_pose/transforms.hpp"
#include "aruco_pose/visualizer.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    const std::string configPath = (argc > 1) ? argv[1] : "config/app.yaml";

    try {
        const aruco_pose::AppConfig config = aruco_pose::loadConfig(configPath);

        const aruco_pose::CameraIntrinsics intrinsics = aruco_pose::loadIntrinsics(config.intrinsicsFile);

        // TODO (Phase 6): build PoseFilter.
        const aruco_pose::MarkerDetector detector;  // DICT_6X6_250 by default
        const aruco_pose::PoseEstimator estimator(intrinsics, config.markerLengthM);
        const aruco_pose::Visualizer visualizer(intrinsics, config.markerLengthM / 2);
        aruco_pose::FpsCounter fps;

        cv::VideoCapture cap(config.cameraIndex, cv::CAP_DSHOW);
        if (!cap.isOpened()) {
            std::cerr << "Error: could not open camera " << config.cameraIndex << ".\n";
            return 1;
        }

        // A request, not a guarantee: the driver picks the closest mode it supports.
        cap.set(cv::CAP_PROP_FRAME_WIDTH, config.frameWidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.frameHeight);
        std::cout << "Requested " << config.frameWidth << "x" << config.frameHeight
                  << ", got " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << "x"
                  << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << "\n";

        std::cout << "Press 'q' or ESC to quit.\n";

        cv::Mat frame;
        while (true) {
            cap >> frame;
            if (frame.empty()) {
                std::cerr << "Error: blank frame grabbed.\n";
                break;
            }

            // fx, fy, cx, cy are in pixels at the calibration resolution. At any other
            // resolution every distance would be wrong, so refuse instead of lying.
            if (frame.size() != intrinsics.imageSize) {
                std::cerr << "Error: camera gives " << frame.cols << "x" << frame.rows
                          << " but intrinsics were calibrated at " << intrinsics.imageSize.width << "x"
                          << intrinsics.imageSize.height << ". Recalibrate or fix frame_width/height.\n";
                return 1;
            }

            const std::vector<aruco_pose::MarkerDetection> detections = detector.detect(frame);

            std::vector<aruco_pose::MarkerPose> poses;
            for (const aruco_pose::MarkerDetection& detection : detections) {
                if (const auto pose = estimator.estimate(detection)) {
                    poses.push_back(*pose);
                }
            }
            // TODO (Phase 6): pose = filter.update(pose)
            // TODO (Phase 5): if two markers are visible, show their relative distance

            // Draw after detecting, so the overlay never confuses the detector.
            visualizer.drawDetections(frame, detections);
            for (size_t i = 0; i < poses.size(); ++i) {
                visualizer.drawAxes(frame, poses[i]);
                // Stack one text block per marker down the left side, below the FPS.
                visualizer.drawPoseText(frame, poses[i], cv::Point(10, 65 + static_cast<int>(i) * 55));
            }

            fps.tick();
            visualizer.drawFps(frame, fps.fps());

            cv::imshow("ArUco Pose", frame);

            const int key = cv::waitKey(1);
            if (key == 'q' || key == 27) {
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
