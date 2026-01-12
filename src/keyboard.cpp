#include "keyboard.h"
#include <conio.h>
#include <iostream>

Keyboard::Keyboard(Memory& memory) : memory(memory), cursor(VideoDevice::VIDEO_START_ADDR) {}

void Keyboard::processInput() {
    // Aktivno čitanje tastaturnih karaktera i ispis na ekran
    if (_kbhit()) {
        char key = _getch();
        
        switch (key) {
            case '\b': // Backspace
                if (cursor > VideoDevice::VIDEO_START_ADDR) {
                    cursor--;
                    memory.write(cursor, ' ');
                }
                break;
            
            case '\r': // Enter
                cursor += VideoDevice::COLS - ((cursor - VideoDevice::VIDEO_START_ADDR) % VideoDevice::COLS);
                if (cursor >= VideoDevice::VIDEO_START_ADDR + VideoDevice::VIDEO_SIZE) {
                    cursor = VideoDevice::VIDEO_START_ADDR;
                }
                break;
            
            default:
                if (key >= 32 && key < 127) { // Printable ASCII
                    memory.write(cursor, (Word)key);
                    cursor++;
                    if (cursor >= VideoDevice::VIDEO_START_ADDR + VideoDevice::VIDEO_SIZE) {
                        cursor = VideoDevice::VIDEO_START_ADDR;
                    }
                }
                break;
        }
    }
}