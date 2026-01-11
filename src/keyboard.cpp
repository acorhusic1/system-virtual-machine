#include "keyboard.h"
#include <conio.h>
#include <iostream>
#include "memory.h"
#include "video.h"

Keyboard::Keyboard(Memory& memory) : memory(memory), cursor(VideoDevice::VIDEO_START_ADDR) {}

void Keyboard::processInput() {
    if (_kbhit()) {
        char key = _getch(); // Read the pressed key

        switch (key) {
            case '\b': // Handle backspace
                if (cursor > VideoDevice::VIDEO_START_ADDR) {
                    cursor--;
                    memory.write(cursor, ' '); // Clear the previous character
                }
                break;

            case 0x4B: // Left arrow (extended key)
                if (cursor > VideoDevice::VIDEO_START_ADDR) {
                    cursor--;
                    memory.write(cursor, memory.read(cursor)); // Refresh cursor position
                }
                break;

            case 0x4D: // Right arrow (extended key)
                if (cursor < VideoDevice::VIDEO_START_ADDR + VideoDevice::VIDEO_SIZE - 1) {
                    cursor++;
                    memory.write(cursor, memory.read(cursor)); // Refresh cursor position
                }
                break;

            case '\r': // Enter key
                cursor += VideoDevice::COLS - (cursor % VideoDevice::COLS);
                if (cursor >= VideoDevice::VIDEO_START_ADDR + VideoDevice::VIDEO_SIZE) {
                    cursor = VideoDevice::VIDEO_START_ADDR; // Wrap around if screen is full
                }
                memory.write(cursor, memory.read(cursor)); // Refresh cursor position
                break;

            default:
                if (key >= 32 && key < 127) { // Printable ASCII characters
                    memory.write(cursor, key);
                    cursor++;
                    if (cursor >= VideoDevice::VIDEO_START_ADDR + VideoDevice::VIDEO_SIZE) {
                        cursor = VideoDevice::VIDEO_START_ADDR; // Wrap around if screen is full
                    }
                }
                break;
        }
    }
}