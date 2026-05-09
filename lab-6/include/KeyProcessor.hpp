#pragma once

enum class Mode {
    NORMAL,
    INVERT,
    BLUR,
    CANNY,
    SOBEL,    // Додано: Фільтр Собеля
    BINARIZE, // Додано: Бінаризація
    GLITCH    // Додано: Glitch effect
};

class KeyProcessor {
private:
    Mode currentMode;
    bool isRunning;
public:
    KeyProcessor();
    void processKey(int key);
    Mode getMode() const;
    bool shouldRun() const;
};