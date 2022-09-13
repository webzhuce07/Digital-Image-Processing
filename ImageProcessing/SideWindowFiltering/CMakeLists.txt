# project name
PROJECT(SideWindowFiltering)
# requirement of cmake version
cmake_minimum_required(VERSION 3.5)

# set the directory of executable files
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Camshift_SOURCE_DIR}/bin)

# find required opencv
find_package(OpenCV REQUIRED)

# directory of opencv headers
include_directories(${OpenCV_INCLUDE_DIRS})
# name of executable file and path of source file
add_executable(SideWindowFiltering main.cpp box_filter.h box_filter.cpp
               median_filter.h median_filter.cpp side_window_box_filter.h
			   side_window_box_filter.cpp side_window_median_filter.h
			   side_window_median_filter.cpp)
# directory of opencv library
link_directories(${OpenCV_LIBRARY_DIRS})
# opencv libraries
target_link_libraries(SideWindowFiltering ${OpenCV_LIBS})
