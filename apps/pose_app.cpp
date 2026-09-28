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

        // TODO (Phase 4): load intrinsics (loadIntrinsics) and pass them in here.
        // TODO: build MarkerDetector, PoseEstimator, PoseFilter.
        aruco_pose::Visualizer visualizer(aruco_pose::CameraIntrinsics{}, config.markerLengthM / 2);
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

            // TODO: pipeline, one step at a time:
            // 1. detections = detector.detect(frame)
            // 2. for each detection: pose = estimator.estimate(detection)
            // 3. (Phase 6) pose = filter.update(pose)
            // 4. draw detections, axes, pose text
            // 5. (Phase 5) if two markers are visible, show their relative distance

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
