# requirement of cmake version
cmake_minimum_required(VERSION 2.8)
# project name
project(RecursiveGaussian)
# find required opencv
find_package(OpenCV REQUIRED)
# name of executable file and path of source file
add_executable(RecursiveGaussian main.cpp)
# opencv libraries
target_link_libraries(RecursiveGaussian ${OpenCV_LIBS})
