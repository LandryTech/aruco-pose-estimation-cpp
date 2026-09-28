#pragma once

#include <string>

namespace aruco_pose {

// All tunable settings, loaded from config/app.yaml.
// Keeping numbers here (not in code) means you never recompile to change them.
struct AppConfig {
    int cameraIndex = 0;
    int frameWidth = 1280;
    int frameHeight = 720;

    int boardCols = 9;           // inner corners across
    int boardRows = 6;           // inner corners down
    double squareSizeM = 0.025;  // meters

    double markerLengthM = 0.080;  // meters

    std::string intrinsicsFile = "config/camera_intrinsics.yaml";
};

// Reads the YAML file at `path`. Throws std::runtime_error if it can't be opened.
AppConfig loadConfig(const std::string& path);

}  // namespace aruco_pose
