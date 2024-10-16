cmake_minimum_required(VERSION 3.7.0)
project(yolo_onnx_cmx)

# C++选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置不同模式下的OpenCV库文件
set(OpenCV_LIBRARIES_DEBUG opencv_world4100d)
set(OpenCV_LIBRARIES_RELEASE opencv_world4100)
set(QT_INCLUDE D:/QT_6/6.8.0/msvc2022_64/include)

# QT的配置
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
include(${Qt6_DIR}/Qt6Config.cmake)
message(STATUS "Qt6 DIR : ${Qt6_DIR}")
set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets)
# QT三个自动选项
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_library(DARKNET_LIBRARY NAMES darknet PATHS /lib REQUIRED)
find_library(ONNX_LIBRARY NAMES onnxruntime PATHS /lib REQUIRED)

# 根据构建模式选择不同的OpenCV库
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(OpenCV_LIBRARIES ${OpenCV_LIBRARIES_DEBUG})
else ()
    set(OpenCV_LIBRARIES ${OpenCV_LIBRARIES_RELEASE})
endif ()

message(STATUS "ONNX_LIBRARY: ${ONNX_LIBRARY}")
message(STATUS "DARKNET_LIBRARY: ${DARKNET_LIBRARY}")
message(STATUS "OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}")

#查找ffmpeg
find_library(FFMPEG_AVCODEC_LIBRARY NAMES avcodec PATHS /lib REQUIRED)
find_library(FFMPEG_AVFORMAT_LIBRARY NAMES avformat PATHS /lib REQUIRED)
find_library(FFMPEG_AVUTIL_LIBRARY NAMES avutil PATHS /lib REQUIRED)
find_library(FFMPEG_SWSCALE_LIBRARY NAMES swscale PATHS /lib REQUIRED)
find_library(FFMPEG_SWRESAMPLE_LIBRARY NAMES swresample PATHS /lib REQUIRED)
find_library(FFMPEG_AVDEVICE_LIBRARY NAMES avdevice PATHS /lib REQUIRED)

set(FFMPEG_LIBRARY ${FFMPEG_AVCODEC_LIBRARY} ${FFMPEG_AVFORMAT_LIBRARY} ${FFMPEG_AVUTIL_LIBRARY} ${FFMPEG_SWSCALE_LIBRARY} ${FFMPEG_SWRESAMPLE_LIBRARY} ${FFMPEG_AVDEVICE_LIBRARY})
message(STATUS "FFmpeg Libraries: ${FFMPEG_LIBRARY}")


include_directories(
        include
        ${ONNXRUNTIME_DIR}/include
        QT_INCLUDE
)

link_directories("${ONNXRUNTIME_DIR}/lib")

set(ALL_LIB
        ${DARKNET_LIBRARY}
        ${ONNX_LIBRARY}
        ${QT_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${FFMPEG_LIBRARY}
)
#————————————————————————————————————————具体的可执行文件配置———————————————————————————————————————————#
# 这个是我的命令行程序，需要自己传参去做识别
set(command_app
        src/main.cpp
        src/detector.cpp
        src/utils.cpp)

# 这个是我的QT推理APP程序，目前我做的是和FFMPEG进程分离的
set(Qt_app
        src/InterFace.cpp
        src/detector.cpp
        src/utils.cpp
        qsrc/userinterface.cpp
        qsrc/userinterface.hpp
        src/putImg.cpp
        src/putImg.hpp
)

# FFMPEG推流用的
set(FFMPEG_APP_SEND
        ffmpeg_src/ffmpeg_utils.hpp
        ffmpeg_src/ffmpeg_sendStream.cpp
        include/LockFreeQueue.h
)

# FFMPEG拉流用的
set(FFMPEG_APP_RECV
        ffmpeg_src/ffmpeg_utils.hpp
        ffmpeg_src/ffmpeg_getStream.cpp
        include/LockFreeQueue.h
)

add_executable(yolo_command ${command_app})

target_link_libraries(yolo_command ${ALL_LIB})

add_executable(Qt_app ${Qt_app})

target_include_directories(Qt_app PRIVATE qsrc src)

target_link_libraries(Qt_app ${ALL_LIB})

add_executable(FFMPEG_APP_SEND ${FFMPEG_APP_SEND})

target_include_directories(FFMPEG_APP_SEND PRIVATE include/ffmpeg)

target_link_libraries(FFMPEG_APP_SEND ${ALL_LIB})

add_executable(FFMPEG_APP_RECV ${FFMPEG_APP_RECV})
target_include_directories(FFMPEG_APP_RECV PRIVATE include/ffmpeg)
target_link_libraries(FFMPEG_APP_RECV ${ALL_LIB})