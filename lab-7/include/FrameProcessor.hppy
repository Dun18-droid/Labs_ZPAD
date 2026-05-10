#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    cv::Mat process(const cv::Mat& inputFrame, Mode mode, int brightness);
private:
    void addFPSCounter(cv::Mat& frame);
    int64 prevTime = 0;
};
