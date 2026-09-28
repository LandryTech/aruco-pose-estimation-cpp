cmake_minimum_required(VERSION 3.16)
project(aruco_pose_estimation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Point directly to OpenCV if CMake fails to auto-detect it
set(OpenCV_DIR "C:/opencv/build" CACHE PATH "Path to OpenCV build folder")
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

# All the vision code lives in one library. Both apps link it,
# so the code is compiled once and shared.
add_library(aruco_pose STATIC
    src/calibration.cpp
    src/config.cpp
    src/fps_counter.cpp
    src/marker_detector.cpp
    src/pose_estimator.cpp
    src/pose_filter.cpp
    src/transforms.cpp
    src/visualizer.cpp
)

# PUBLIC: anything that links aruco_pose also gets these include paths and libs.
target_include_directories(aruco_pose PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(aruco_pose SYSTEM PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(aruco_pose PUBLIC ${OpenCV_LIBS})

if(MSVC)
    target_compile_options(aruco_pose PUBLIC /W4 /permissive-)
else()
    target_compile_options(aruco_pose PUBLIC -Wall -Wextra -Wpedantic)
endif()

add_executable(calibrate_camera apps/calibrate_camera.cpp)
target_link_libraries(calibrate_camera PRIVATE aruco_pose)

add_executable(pose_app apps/pose_app.cpp)
target_link_libraries(pose_app PRIVATE aruco_pose)
