#include "FrameProcessor.hpp"

cv::Mat FrameProcessor::process(const cv::Mat& inputFrame, Mode mode, int brightness) {
    if (inputFrame.empty()) return inputFrame;

    cv::Mat outputFrame;

    // Зміна яскравості (інтерактивний елемент впливає на кадр)[cite: 1]
    inputFrame.convertTo(outputFrame, -1, 1, brightness - 50); 

    switch (mode) {
        case Mode::NORMAL:
            // Вже застосовано яскравість
            break;
        case Mode::INVERT:
            cv::bitwise_not(outputFrame, outputFrame); // Інверсія кольорів (bitwise_not)[cite: 1]
            break;
        case Mode::BLUR:
            cv::GaussianBlur(outputFrame, outputFrame, cv::Size(15, 15), 0); // Gaussian blur[cite: 1]
            break;
        case Mode::CANNY:
            cv::cvtColor(outputFrame, outputFrame, cv::COLOR_BGR2GRAY);
            cv::Canny(outputFrame, outputFrame, 50, 150); // Canny фільтр[cite: 1]
            cv::cvtColor(outputFrame, outputFrame, cv::COLOR_GRAY2BGR); // Повертаємо 3 канали для одноманітного виводу
            break;
    }

    addFPSCounter(outputFrame);
    return outputFrame;
}

void FrameProcessor::addFPSCounter(cv::Mat& frame) {
    int64 currentTime = cv::getTickCount();
    double fps = cv::getTickFrequency() / (currentTime - prevTime);
    prevTime = currentTime;

    // Відображення тексту: лічильник FPS[cite: 1]
    std::string fpsText = "FPS: " + std::to_string((int)fps);
    cv::putText(frame, fpsText, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
}
