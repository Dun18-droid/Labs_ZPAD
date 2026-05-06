#include "KeyProcessor.hpp"

KeyProcessor::KeyProcessor() : currentMode(Mode::NORMAL), isRunning(true) {}

void KeyProcessor::processKey(int key) {
    if (key == -1) return; 

    switch (key) {
        case 27: // ESC
        case 'q':
            isRunning = false;
            break;
        case '1':
            currentMode = Mode::NORMAL;
            break;
        case '2':
            currentMode = Mode::INVERT; // Режим інверсії кольорів[cite: 1]
            break;
        case '3':
            currentMode = Mode::BLUR; // Режим Gaussian blur[cite: 1]
            break;
        case '4':
            currentMode = Mode::CANNY; // Режим Canny фільтру[cite: 1]
            break;
    }
}

Mode KeyProcessor::getMode() const { return currentMode; }
bool KeyProcessor::shouldRun() const { return isRunning; }
