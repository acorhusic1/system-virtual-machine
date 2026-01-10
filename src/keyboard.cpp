#include "keyboard.h"

Word KeyboardDevice::readPort(Word port) {
    // Vratit ćemo 0 (nema pritisnutih tipki) za početak
    return 0;
}

void KeyboardDevice::setKeyState(int row, int bit, bool pressed) {
    // Implementirat ćemo kasnije
}