#include "FrameProcessor.hpp"
#include <vector>

cv::Mat FrameProcessor::process(const cv::Mat& inputFrame, Mode mode, int brightness) {
    if (inputFrame.empty()) return inputFrame;

    cv::Mat outputFrame;

    // Зміна яскравості (інтерактивний елемент впливає на кадр) 
    inputFrame.convertTo(outputFrame, -1, 1, brightness - 50); 

    switch (mode) {
        case Mode::NORMAL:
            break;
        case Mode::INVERT:
            cv::bitwise_not(outputFrame, outputFrame); // Інверсія кольорів (bitwise_not) 
            break;
        case Mode::BLUR:
            cv::GaussianBlur(outputFrame, outputFrame, cv::Size(15, 15), 0); // Gaussian blur 
            break;
        case Mode::CANNY:
            cv::cvtColor(outputFrame, outputFrame, cv::COLOR_BGR2GRAY);
            cv::Canny(outputFrame, outputFrame, 50, 150); // Canny фільтр 
            cv::cvtColor(outputFrame, outputFrame, cv::COLOR_GRAY2BGR); 
            break;
            
        //НОВІ ФІЛЬТРИ
        
        case Mode::SOBEL: {
            cv::Mat gray, grad_x, grad_y, abs_grad_x, abs_grad_y;
            cv::cvtColor(outputFrame, gray, cv::COLOR_BGR2GRAY);
            cv::Sobel(gray, grad_x, CV_16S, 1, 0, 3);
            cv::Sobel(gray, grad_y, CV_16S, 0, 1, 3);
            cv::convertScaleAbs(grad_x, abs_grad_x);
            cv::convertScaleAbs(grad_y, abs_grad_y);
            cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, outputFrame);
            cv::cvtColor(outputFrame, outputFrame, cv::COLOR_GRAY2BGR);
            break;
        }
        case Mode::BINARIZE: { 
            cv::cvtColor(outputFrame, outputFrame, cv::COLOR_BGR2GRAY);
            cv::threshold(outputFrame, outputFrame, 128, 255, cv::THRESH_BINARY);
            cv::cvtColor(outputFrame, outputFrame, cv::COLOR_GRAY2BGR);
            break;
        }
        case Mode::GLITCH: {
            std::vector<cv::Mat> channels;
            cv::split(outputFrame, channels);
            
            int shift = 15;

            cv::Mat shiftedR = cv::Mat::zeros(channels[2].size(), channels[2].type());
            channels[2](cv::Rect(0, 0, channels[2].cols - shift, channels[2].rows))
                .copyTo(shiftedR(cv::Rect(shift, 0, channels[2].cols - shift, channels[2].rows)));
            channels[2] = shiftedR;

            cv::Mat shiftedB = cv::Mat::zeros(channels[0].size(), channels[0].type());
            channels[0](cv::Rect(shift, 0, channels[0].cols - shift, channels[0].rows))
                .copyTo(shiftedB(cv::Rect(0, 0, channels[0].cols - shift, channels[0].rows)));
            channels[0] = shiftedB;

            cv::merge(channels, outputFrame);
            break;
        }
    }

    addFPSCounter(outputFrame);
    return outputFrame;
}

void FrameProcessor::addFPSCounter(cv::Mat& frame) {
    int64 currentTime = cv::getTickCount();
    double fps = cv::getTickFrequency() / (currentTime - prevTime);
    prevTime = currentTime;

    // Відображення тексту: лічильник FPS 
    std::string fpsText = "FPS: " + std::to_string((int)fps);
    cv::putText(frame, fpsText, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
}