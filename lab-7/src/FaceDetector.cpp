#include "FaceDetector.hpp"
#include <iostream>
#include <chrono>

FaceDetector::FaceDetector(const std::string& prototxt, const std::string& model) {
    try {
        // Завантаження мережі [cite: 20]
        net = cv::dnn::readNetFromCaffe(prototxt, model);
    } catch (const cv::Exception& e) {
        std::cerr << "Помилка завантаження моделі: " << e.what() << std::endl;
    }
    
    isRunning = true;
    hasNewFrame = false;
    // Запуск фонового потоку
    workerThread = std::thread(&FaceDetector::workerLoop, this);
}

FaceDetector::~FaceDetector() {
    isRunning = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void FaceDetector::processFrameAsync(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(mtx);
    currentFrame = frame.clone();
    hasNewFrame = true;
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(mtx);
    return detectedFaces;
}

void FaceDetector::workerLoop() {
    while (isRunning) {
        cv::Mat frameToProcess;
        bool processNow = false;

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (hasNewFrame && !currentFrame.empty()) {
                frameToProcess = currentFrame.clone();
                hasNewFrame = false;
                processNow = true;
            }
        }

        if (processNow) {
            // Перетворення кадру у формат 300x300 для нейромережі [cite: 23, 24, 25]
            cv::Mat blob = cv::dnn::blobFromImage(frameToProcess, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
            net.setInput(blob); // [cite: 26]
            
            // Інференс [cite: 27, 28]
            cv::Mat detections = net.forward();

            std::vector<cv::Rect> newFaces;
            cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

            // Фільтрація облич з confidence > 50% [cite: 29]
            for (int i = 0; i < detectionMat.rows; i++) {
                float confidence = detectionMat.at<float>(i, 2);
                if (confidence > 0.5) {
                    int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameToProcess.cols);
                    int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameToProcess.rows);
                    int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameToProcess.cols);
                    int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameToProcess.rows);
                    newFaces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
                }
            }
            // std::this_thread::sleep_for(std::chrono::milliseconds(500)); 

            {
                std::lock_guard<std::mutex> lock(mtx);
                detectedFaces = newFaces; // Оновлюємо координати [cite: 51]
            }
        } else {
            // Щоб не перевантажувати процесор порожніми циклами
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}