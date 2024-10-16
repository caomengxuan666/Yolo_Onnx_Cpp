//
// Created by CaoMengxuan on 2024/9/19.
//

#include "putImg.hpp"

putImg::putImg() {
    //isGPU=true;
    //detector=YOLODetector(isGPU);
    classNames=utils::loadNames("../models/coco.names");
    modelPath="yolov5m.onnx";
    //setModelPath(modelPath);
    //detector.loadModel(modelPath);
    isInit= true;
    detector.loadModel(modelPath);
    std::cerr<<"NOW using ModelPath"<<modelPath<<std::endl;
}

putImg::putImg(const std::string &modelPath, const std::string &imagePath,
               const std::string &classNamesPath, bool isGPU,
               float confThreshold, float iouThreshold, YOLODetector detector) {

    classNames = utils::loadNames(classNamesPath);

    if (classNames.empty()) {
        std::cerr << "Error: Empty class names file." << std::endl;
        return;
    }

    detector = YOLODetector(modelPath, isGPU, cv::Size(640, 640));

    detector.loadModel(modelPath);
    isInit= true;
}

void putImg::reloadClassFile() {
    classNames = utils::loadNames(classNamesPath);

    if (classNames.empty()) {
        std::cerr << "Error: Empty class names file." << std::endl;
        return;
    }
}

void putImg::setModelPath(const std::string &modelPath_) {
    if(modelPath_== modelPath)return;
    modelPath = modelPath_;
    detector.loadModel(modelPath);
    std::cerr<<"NOW using ModelPath"<<modelPath<<std::endl;
}
void putImg::setImagePath(const std::string &imagePath_) {
    imagePath = imagePath_;
}
void putImg::setClassNamesPath(const std::string &classNamesPath_) {
    classNamesPath = classNamesPath_;
    reloadClassFile();
}
void putImg::setIsGPU(bool isGPU_) {
    if(isGPU_== isGPU) return;
    isGPU = isGPU_;
    std::cout<<"ReInternal device by"<<__FUNCTION__ <<std::endl;
    detector=YOLODetector(isGPU_);
}
void putImg::setConfThreshold(float confThreshold_) {
    confThreshold = confThreshold_;
}
void putImg::setIouThreshold(float iouThreshold_) {
    iouThreshold = iouThreshold_;
}

QImage putImg::getImage() {
    setModelPath(modelPath);

    try {
        std::cout << "Model was initialized." << std::endl;
        image = cv::imread(imagePath);
        result = detector.detect(image, confThreshold, iouThreshold);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return utils::getImg(image, result, classNames);
}
