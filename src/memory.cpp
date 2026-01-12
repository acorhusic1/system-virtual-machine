#include "memory.h"
#include <iostream>
#include <windows.h>

Memory::Memory() {
    data.assign(65536, 0);
    video.init();
}

Word Memory::read(Word address) {
    // TASTATURA (Specifikacija: 0xFF00 - 0xFF05)
    if (address >= 0xFF00 && address <= 0xFF05) {
        Word mask = 0;
        
        switch (address) {
            // PORT 0xFF00: ESC, F1-F12
            case 0xFF00: {
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) mask |= (1 << 0);
                if (GetAsyncKeyState(VK_F1) & 0x8000) mask |= (1 << 1);
                if (GetAsyncKeyState(VK_F2) & 0x8000) mask |= (1 << 2);
                if (GetAsyncKeyState(VK_F3) & 0x8000) mask |= (1 << 3);
                if (GetAsyncKeyState(VK_F4) & 0x8000) mask |= (1 << 4);
                if (GetAsyncKeyState(VK_F5) & 0x8000) mask |= (1 << 5);
                if (GetAsyncKeyState(VK_F6) & 0x8000) mask |= (1 << 6);
                if (GetAsyncKeyState(VK_F7) & 0x8000) mask |= (1 << 7);
                if (GetAsyncKeyState(VK_F8) & 0x8000) mask |= (1 << 8);
                if (GetAsyncKeyState(VK_F9) & 0x8000) mask |= (1 << 9);
                if (GetAsyncKeyState(VK_F10) & 0x8000) mask |= (1 << 10);
                if (GetAsyncKeyState(VK_F11) & 0x8000) mask |= (1 << 11);
                if (GetAsyncKeyState(VK_F12) & 0x8000) mask |= (1 << 12);
                break;
            }
            
            // PORT 0xFF01: 1-9, 0, ', +, Backspace
            case 0xFF01: {
                if (GetAsyncKeyState('1') & 0x8000) mask |= (1 << 0);
                if (GetAsyncKeyState('2') & 0x8000) mask |= (1 << 1);
                if (GetAsyncKeyState('3') & 0x8000) mask |= (1 << 2);
                if (GetAsyncKeyState('4') & 0x8000) mask |= (1 << 3);
                if (GetAsyncKeyState('5') & 0x8000) mask |= (1 << 4);
                if (GetAsyncKeyState('6') & 0x8000) mask |= (1 << 5);
                if (GetAsyncKeyState('7') & 0x8000) mask |= (1 << 6);
                if (GetAsyncKeyState('8') & 0x8000) mask |= (1 << 7);
                if (GetAsyncKeyState('9') & 0x8000) mask |= (1 << 8);
                if (GetAsyncKeyState('0') & 0x8000) mask |= (1 << 9);
                if (GetAsyncKeyState(VK_OEM_7) & 0x8000) mask |= (1 << 10);  // '
                if (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000) mask |= (1 << 11); // +
                if (GetAsyncKeyState(VK_BACK) & 0x8000) mask |= (1 << 12);  // Backspace
                break;
            }
            
            // PORT 0xFF02: Tab, Q-P, [, ], Enter
            case 0xFF02: {
                if (GetAsyncKeyState(VK_TAB) & 0x8000) mask |= (1 << 0);
                if (GetAsyncKeyState('Q') & 0x8000) mask |= (1 << 1);
                if (GetAsyncKeyState('W') & 0x8000) mask |= (1 << 2);
                if (GetAsyncKeyState('E') & 0x8000) mask |= (1 << 3);
                if (GetAsyncKeyState('R') & 0x8000) mask |= (1 << 4);
                if (GetAsyncKeyState('T') & 0x8000) mask |= (1 << 5);
                if (GetAsyncKeyState('Y') & 0x8000) mask |= (1 << 6);
                if (GetAsyncKeyState('U') & 0x8000) mask |= (1 << 7);
                if (GetAsyncKeyState('I') & 0x8000) mask |= (1 << 8);
                if (GetAsyncKeyState('O') & 0x8000) mask |= (1 << 9);
                if (GetAsyncKeyState('P') & 0x8000) mask |= (1 << 10);
                if (GetAsyncKeyState(VK_OEM_4) & 0x8000) mask |= (1 << 11);  // [
                if (GetAsyncKeyState(VK_OEM_6) & 0x8000) mask |= (1 << 12);  // ]
                if (GetAsyncKeyState(VK_RETURN) & 0x8000) mask |= (1 << 13);
                break;
            }
            
            // PORT 0xFF03: Caps Lock, A-L, ;, ", Ž
            case 0xFF03: {
                if (GetAsyncKeyState(VK_CAPITAL) & 0x0001) mask |= (1 << 0);  // Caps Lock (toggle state)
                if (GetAsyncKeyState('A') & 0x8000) mask |= (1 << 1);
                if (GetAsyncKeyState('S') & 0x8000) mask |= (1 << 2);
                if (GetAsyncKeyState('D') & 0x8000) mask |= (1 << 3);
                if (GetAsyncKeyState('F') & 0x8000) mask |= (1 << 4);
                if (GetAsyncKeyState('G') & 0x8000) mask |= (1 << 5);
                if (GetAsyncKeyState('H') & 0x8000) mask |= (1 << 6);
                if (GetAsyncKeyState('J') & 0x8000) mask |= (1 << 7);
                if (GetAsyncKeyState('K') & 0x8000) mask |= (1 << 8);
                if (GetAsyncKeyState('L') & 0x8000) mask |= (1 << 9);
                if (GetAsyncKeyState(VK_OEM_1) & 0x8000) mask |= (1 << 10);  // ;
                if (GetAsyncKeyState(VK_OEM_7) & 0x8000) mask |= (1 << 11);  // "
                if (GetAsyncKeyState(VK_OEM_3) & 0x8000) mask |= (1 << 12);  // Ž (aproksimacija)
                break;
            }
            
            // PORT 0xFF04: Z-M, ,, ., /, Left Shift, Right Shift
            case 0xFF04: {
                if (GetAsyncKeyState('Z') & 0x8000) mask |= (1 << 0);
                if (GetAsyncKeyState('X') & 0x8000) mask |= (1 << 1);
                if (GetAsyncKeyState('C') & 0x8000) mask |= (1 << 2);
                if (GetAsyncKeyState('V') & 0x8000) mask |= (1 << 3);
                if (GetAsyncKeyState('B') & 0x8000) mask |= (1 << 4);
                if (GetAsyncKeyState('N') & 0x8000) mask |= (1 << 5);
                if (GetAsyncKeyState('M') & 0x8000) mask |= (1 << 6);
                if (GetAsyncKeyState(VK_OEM_COMMA) & 0x8000) mask |= (1 << 7);  // ,
                if (GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000) mask |= (1 << 8); // .
                if (GetAsyncKeyState(VK_OEM_2) & 0x8000) mask |= (1 << 9);      // /
                if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) mask |= (1 << 10);
                if (GetAsyncKeyState(VK_RSHIFT) & 0x8000) mask |= (1 << 11);
                break;
            }
            
            // PORT 0xFF05: Left Ctrl, Right Ctrl, Space, Alt, AltGr
            case 0xFF05: {
                if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) mask |= (1 << 0);
                if (GetAsyncKeyState(VK_RCONTROL) & 0x8000) mask |= (1 << 1);
                if (GetAsyncKeyState(VK_SPACE) & 0x8000) mask |= (1 << 2);
                if (GetAsyncKeyState(VK_LMENU) & 0x8000) mask |= (1 << 3);     // Alt
                if (GetAsyncKeyState(VK_RMENU) & 0x8000) mask |= (1 << 4);     // AltGr
                break;
            }
        }
        
        return mask;
    }

    // DISK (Specifikacija: 0xFFFC prenos podataka)
    if (address == 0xFFFC) {
        return disk.readPort(address);
    }

    // Normalna RAM/ROM memorija
    return data[address];
}

void Memory::write(Word address, Word value) {
    // ROM ZAŠTITA - ROM je read-only (adrese 0-2047)
    if (address < 2048) {
        return;  // ROM je zaštićen
    }

    // VIDEO RAM (Specifikacija: adresa 8192, 1 riječ = 1 ASCII)
    if (address >= 8192 && address < 10192) {
        video.writeChar(address, value);
        data[address] = value;
        return;
    }

    // DISK (Specifikacija: 0xFFFE komanda, 0xFFFD sektor, 0xFFFC data)
    if (address >= 0xFFFC && address <= 0xFFFE) {
        disk.writePort(address, value);
        return;
    }

    // Normalno pisanje u RAM
    if (address < 65536) {
        data[address] = value;
    }
}

bool Memory::loadFromFile(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) return false;
    
    // SVEU16 instrukcije su 16-bitne (Big Endian u fajlu)
    uint8_t buffer[2];
    int addr = 0;
    while (fread(buffer, 1, 2, file) == 2 && addr < 65536) {
        data[addr++] = (buffer[0] << 8) | buffer[1];
    }
    fclose(file);
    return true;
}

Word* Memory::getRawPointer(Word address) {
    return &data[address];
}