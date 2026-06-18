#pragma once

enum class Mode {
    NORMAL,
    INVERT,
    BLUR,
    CANNY,
    SOBEL,    
    BINARIZE, 
    GLITCH    
};

class KeyProcessor {
private:
    Mode currentMode;
    bool isRunning;
    bool faceDetectionEnabled; // Новий режим обробки (вмикається клавішею F) [cite: 10]
public:
    KeyProcessor();
    void processKey(int key);
    Mode getMode() const;
    bool shouldRun() const;
    bool isFaceDetectionEnabled() const;
};