#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

class FaceDetector {
private:
    cv::dnn::Net net;
    std::thread workerThread; // Фоновий потік [cite: 37]
    std::mutex mtx;           // Захист даних [cite: 38]
    std::atomic<bool> isRunning; // Безпечна зупинка [cite: 39]
    std::atomic<bool> hasNewFrame;
    
    cv::Mat currentFrame;
    std::vector<cv::Rect> detectedFaces;

    void workerLoop();

public:
    FaceDetector(const std::string& prototxt, const std::string& model);
    ~FaceDetector();

    void processFrameAsync(const cv::Mat& frame);
    std::vector<cv::Rect> getFaces();
};