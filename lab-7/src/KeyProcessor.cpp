#include "KeyProcessor.hpp"

KeyProcessor::KeyProcessor() : currentMode(Mode::NORMAL), isRunning(true), faceDetectionEnabled(false) {}

void KeyProcessor::processKey(int key) {
    if (key == -1) return; 

    switch (key) {
        case 27: // ESC
        case 'q':
            isRunning = false;
            break;
        case '1': currentMode = Mode::NORMAL; break;
        case '2': currentMode = Mode::INVERT; break;
        case '3': currentMode = Mode::BLUR; break;
        case '4': currentMode = Mode::CANNY; break;
        case '5': currentMode = Mode::SOBEL; break;
        case '6': currentMode = Mode::BINARIZE; break;
        case '7': currentMode = Mode::GLITCH; break;
        case 'f': 
        case 'F': 
            faceDetectionEnabled = !faceDetectionEnabled; // Перемикач детекції [cite: 10]
            break;
    }
}

Mode KeyProcessor::getMode() const { return currentMode; }
bool KeyProcessor::shouldRun() const { return isRunning; }
bool KeyProcessor::isFaceDetectionEnabled() const { return faceDetectionEnabled; }