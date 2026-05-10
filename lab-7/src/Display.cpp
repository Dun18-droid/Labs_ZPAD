#include "Display.hpp"

Display::Display(const std::string& name) : windowName(name) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

void Display::show(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::imshow(windowName, frame); // Відображає результат після обробки (cv::imshow)[cite: 1]
    }
}

void Display::createBrightnessTrackbar(int* brightnessValue) {
    // Слайдер, який впливає на яскравість[cite: 1]
    cv::createTrackbar("Brightness", windowName, brightnessValue, 100);
}
