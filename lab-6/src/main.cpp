#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include <iostream>

int main() {
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        return -1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display("Lab 6 - Video Processing");

    int brightness = 50; // Початкове значення (від 0 до 100, де 50 - без змін)
    display.createBrightnessTrackbar(&brightness);

    // Цикл, у якому прописана вся логіка[cite: 1]
    while (keyProcessor.shouldRun()) {
        // Читає відео з камери[cite: 1]
        cv::Mat frame = camera.getFrame(); 
        
        if (frame.empty()) {
            std::cerr << "Порожній кадр!" << std::endl;
            break;
        }

        Mode currentMode = keyProcessor.getMode();
        
        // Здійснює обробку зображень в залежності від режиму[cite: 1]
        cv::Mat processedFrame = frameProcessor.process(frame, currentMode, brightness); 
        
        // Відображає його у вікні[cite: 1]
        display.show(processedFrame); 

        // Реагує на натискання клавіатури[cite: 1]
        int key = cv::waitKey(30) & 0xFF;
        keyProcessor.processKey(key);
    }

    return 0;
}
