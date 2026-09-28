# ArUco 6 DoF Pose Estimation (C++ / OpenCV)

Real-time 6 DoF pose estimation of ArUco markers from a single laptop webcam.

> Work in progress. This README becomes the project report.

## Demo
<!-- GIF here -->

## How it works
<!-- Calibration, detection, PnP, frames. Your own explanation. -->

## Results
<!-- Distance accuracy, angle accuracy, FPS, robustness tables and plots. -->

## Project structure
```
include/aruco_pose/   Public headers, one class per file
src/                  Implementations
apps/                 calibrate_camera and pose_app executables
config/app.yaml       All tunable settings
```

## Build (Windows, MSVC)
Requires OpenCV 4.7+ with `OpenCV_DIR` set and its `bin` folder on PATH.
```
cmake -S . -B build
cmake --build build --config Release
```

## Run (from the repo root)
```
.\build\Release\calibrate_camera.exe
.\build\Release\pose_app.exe
```

## Limitations
<!-- Fill in from testing. -->
