cmake_minimum_required(VERSION 2.8)
project(myCalibCamera)
set(CMAKE_BUILD_TYPE "Debug")
set("OpenCV_DIR" "/usr/local/share/OpenCV")
#set("OpenCV_DIR" "/home/zhaosl/local_install/opencv2.4.9/share/OpenCV")
#set("OpenCV_DIR" "/home/zhaosl/local_install/opencv3.0.0/share/OpenCV")
#set("OpenCV_DIR" "/home/zhaosl/local_install/opencv3.1.0/share/OpenCV")

#set("OpenCV_DIR" "/home/pc2/opencv-3.3.0/build/install/share/OpenCV/")
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
MESSAGE("OpenCV vesion: ${OpenCV_VERSION}")
MESSAGE("OpenCV vesion: ${OpenCV_DIR}")

set(PROJECT_ROOT ".")
include_directories(${PROJECT_ROOT}/include)
set(MODULE_SRC_ROOT ${PROJECT_ROOT}/src/)

file(GLOB src_calib_files ${MODULE_SRC_ROOT}/calibration.cpp)
add_executable(myCalibCamera ${src_calib_files})
target_link_libraries(myCalibCamera ${OpenCV_LIBS} )


file(GLOB src_imglistcreater_files ${MODULE_SRC_ROOT}/imagelist_creator.cpp)
add_executable(myImageListCreater ${src_imglistcreater_files})
target_link_libraries(myImageListCreater ${OpenCV_LIBS} )

file(GLOB src_undistorRectifyMap_files ${MODULE_SRC_ROOT}/undistortRectifyMap.cpp)
add_executable(myUndisRectifyMap ${src_undistorRectifyMap_files})
target_link_libraries(myUndisRectifyMap ${OpenCV_LIBS} )

