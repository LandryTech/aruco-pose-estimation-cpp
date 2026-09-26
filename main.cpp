#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Initialize the camera feed (0 is usually the built-in webcam)
    cv::VideoCapture cap(0, cv::CAP_DSHOW); // CAP_DSHOW or CAP_MSMF speeds up opening on Windows

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera feed." << std::endl;
        return -1;
    }

    std::cout << "Camera opened successfully! Press 'ESC' or 'q' to quit." << std::endl;

    cv::Mat frame;
    while (true) {
        // Read a new frame from the camera
        cap >> frame;

        // Check if frame is empty
        if (frame.empty()) {
            std::cerr << "Error: Blank frame grabbed." << std::endl;
            break;
        }

        // Display the frame in a window named "Camera Feed"
        cv::imshow("Camera Feed", frame);

        // Wait 30ms for a key press; exit if 'q' is pressed
        char key = (char)cv::waitKey(30);
        if (key == 'q') {
            break;
        }
    }

    // Release camera resource
    cap.release();
    cv::destroyAllWindows();

    return 0;
}