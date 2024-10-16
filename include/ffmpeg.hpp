//
// Created by CaoMengxuan on 2024/9/20.
//

#ifndef YOLO_ORT_FFMPEG_HPP
#define YOLO_ORT_FFMPEG_HPP

#ifdef __cplusplus
extern "C" {
#include <ffmpeg/libavcodec/avcodec.h>
#include <ffmpeg/libavdevice/avdevice.h>
#include <ffmpeg/libavfilter/avfilter.h>
#include <ffmpeg/libavformat/avformat.h>
#include <ffmpeg/libavutil/avutil.h>
#include <ffmpeg/libpostproc/postprocess.h>
#include <ffmpeg/libswresample/swresample.h>
#include <ffmpeg/libswscale/swscale.h>
#include "libavutil/imgutils.h"
}
#endif

#endif//YOLO_ORT_FFMPEG_HPP
