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
            currentMode = Mode::INVERT; // Режим інверсії кольорів 
            break;
        case '3':
            currentMode = Mode::BLUR; // Режим Gaussian blur 
            break;
        case '4':
            currentMode = Mode::CANNY; // Режим Canny фільтру 
            break;
        //НОВІ РЕЖИМИ
        case '5':
            currentMode = Mode::SOBEL; // Фільтр Собеля 
            break;
        case '6':
            currentMode = Mode::BINARIZE; // Бінаризація 
            break;
        case '7':
            currentMode = Mode::GLITCH; // Glitch effect 
            break;
    }
}

Mode KeyProcessor::getMode() const { return currentMode; }
bool KeyProcessor::shouldRun() const { return isRunning; }