cmake_minimum_required(VERSION 3.10)
project(aruco_pose_estimation)

set(CMAKE_CXX_STANDARD 17)

# Point directly to OpenCV if CMake fails to auto-detect it
set(OpenCV_DIR "C:/opencv/build" CACHE PATH "Path to OpenCV build folder")
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(aruco_app main.cpp)
target_link_libraries(aruco_app ${OpenCV_LIBS})