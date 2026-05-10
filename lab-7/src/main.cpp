#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp" // Підключаємо наш новий клас
#include <iostream>

int main() {
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        return -1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display("Lab 7 - Video Processing & CV");

    // Ініціалізація детектора. Ваги мають бути завантажені скриптом
    FaceDetector faceDetector("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");

    int brightness = 50; 
    display.createBrightnessTrackbar(&brightness);

    while (keyProcessor.shouldRun()) {
        cv::Mat frame = camera.getFrame(); 
        
        if (frame.empty()) {
            std::cerr << "Порожній кадр!" << std::endl;
            break;
        }

        Mode currentMode = keyProcessor.getMode();
        cv::Mat processedFrame = frameProcessor.process(frame, currentMode, brightness); 
        
        // Логіка Face Detection
        if (keyProcessor.isFaceDetectionEnabled()) {
            // Відправляємо копію кадру у фоновий потік [cite: 43]
            faceDetector.processFrameAsync(frame); 
            
            // Забираємо координати і малюємо рамки [cite: 44, 45]
            std::vector<cv::Rect> faces = faceDetector.getFaces();
            for (const auto& face : faces) {
                cv::rectangle(processedFrame, face, cv::Scalar(0, 255, 0), 2);
            }
        }
        
        display.show(processedFrame); // [cite: 46]

        int key = cv::waitKey(30) & 0xFF;
        keyProcessor.processKey(key);
    }

    return 0;
}