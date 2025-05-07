cmake_minimum_required(VERSION 3.10)
project(Matrix_lib_project)
set(OpenCV_DIR /opt/homebrew/Cellar/opencv/4.11.0_1/lib/cmake/opencv4)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Matrix_lib_project try.cpp matrix_lib.cpp) 
target_link_libraries(Matrix_lib_project ${OpenCV_LIBS})