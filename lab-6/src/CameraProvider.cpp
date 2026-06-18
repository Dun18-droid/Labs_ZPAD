#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider(int deviceId) {
    cap.open(deviceId);
    if (!cap.isOpened()) {
        std::cerr << "Помилка: не вдалося відкрити камеру!" << std::endl;
    }
}

CameraProvider::~CameraProvider() {
    cap.release();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame; // Читає відео з камери
    return frame;
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}
