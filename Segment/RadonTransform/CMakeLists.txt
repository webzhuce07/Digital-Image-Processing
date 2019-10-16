cmake_minimum_required(VERSION 2.6)
project(RadonTransform)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package( OpenCV REQUIRED )

add_executable(radon_transform test.cpp radon_transform.cpp)

target_link_libraries( radon_transform ${OpenCV_LIBS} )
