//
// Created by CaoMengxuan on 2024/9/19.
//

#ifndef YOLO_ORT_PUTIMG_HPP
#define YOLO_ORT_PUTIMG_HPP

#include "utils.h"
#include "detector.h"
#include <QImage>

class putImg {
public:
    putImg();

    putImg(const std::string &modelPath, const std::string &imagePath,
           const std::string &classNamesPath, bool isGPU= false,
           float confThreshold=0.4, float iouThreshold=0.5,YOLODetector detector={});

    void reloadClassFile();

    QImage getImage();

    void setModelPath(const std::string &modelPath_);
    void setImagePath(const std::string &imagePath_);
    void setClassNamesPath(const std::string &classNamesPath_);
    void setIsGPU(bool isGPU_);
    void setConfThreshold(float confThreshold_);
    void setIouThreshold(float iouThreshold_);

private:
    YOLODetector detector;
    std::vector<std::string> classNames;
    float confThreshold;
    float iouThreshold;
    std::string imagePath;
    std::string modelPath;
    std::string classNamesPath;

    bool isInit;
    bool isGPU;
    cv::Mat image;
    std::vector<Detection> result;

};


#endif//YOLO_ORT_PUTIMG_HPP
