#pragma once

enum class Mode {
    NORMAL,
    INVERT,
    BLUR,
    CANNY
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
