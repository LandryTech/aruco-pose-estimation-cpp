#include "aruco_pose/config.hpp"

#include <opencv2/core.hpp>
#include <stdexcept>

namespace aruco_pose {

namespace {

// Reads one key into `value`. Throws if the key is missing, so a typo in
// app.yaml fails loudly instead of silently becoming 0.
template <typename T>
void readRequired(const cv::FileStorage& fs, const std::string& key, T& value) {
    const cv::FileNode node = fs[key];
    if (node.empty()) {
        throw std::runtime_error("Config is missing key: " + key);
    }
    node >> value;
}

}  // namespace

AppConfig loadConfig(const std::string& path) {
    cv::FileStorage fs(path, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        throw std::runtime_error("Could not open config file: " + path +
                                 " (run from the repo root, or pass the path as an argument)");
    }

    AppConfig config;
    readRequired(fs, "camera_index", config.cameraIndex);
    readRequired(fs, "frame_width", config.frameWidth);
    readRequired(fs, "frame_height", config.frameHeight);
    readRequired(fs, "board_cols", config.boardCols);
    readRequired(fs, "board_rows", config.boardRows);
    readRequired(fs, "square_size_m", config.squareSizeM);
    readRequired(fs, "marker_length_m", config.markerLengthM);
    readRequired(fs, "intrinsics_file", config.intrinsicsFile);

    return config;
}

}  // namespace aruco_pose
