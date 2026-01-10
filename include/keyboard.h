#pragma once
#include "common.h"

class KeyboardDevice {
public:
    // Metoda koja će se pozivati kada CPU čita sa portova 0xFF00-0xFF05
    Word readPort(Word port);
    
    // Metoda za simuliranje pritiska tipke (za testiranje)
    void setKeyState(int row, int bit, bool pressed);
};