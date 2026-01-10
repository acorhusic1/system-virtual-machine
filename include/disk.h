#pragma once
#include "common.h"
#include <string>

class DiskDevice {
public:
    // Portovi za disk
    static const Word PORT_CMD = 0xFFFE;
    static const Word PORT_SECTOR = 0xFFFD;
    static const Word PORT_DATA = 0xFFFC;

    DiskDevice();
    void writePort(Word port, Word value);
    Word readPort(Word port);
};