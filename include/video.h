#pragma once
#include "common.h"
#include "memory.h"

class VideoDevice {
public:
    // Video počinje na adresi 8192
    static const Word VIDEO_START_ADDR = 8192;
    static const int ROWS = 25;
    static const int COLS = 80;

    void render(Memory& mem);
};