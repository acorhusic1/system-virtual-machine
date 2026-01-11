#include "memory.h"
#include <iostream>
#include <conio.h>
#include <windows.h> // Za GetAsyncKeyState

Memory::Memory() {
    data.resize(MEM_SIZE, 0);
    // Initialize the video device
    video.init();
}

Word Memory::read(Word address) {
    // Tastatura: Portovi 0xFF00 - 0xFF05
    if (address >= 0xFF00 && address <= 0xFF05) {
        Word rowMask = 0;

        // Port 0xFF00: Prvi red (ESC, F1-F12)
        if (address == 0xFF00) {
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) rowMask |= (1 << 0);  // ESC
            if (GetAsyncKeyState(VK_F1) & 0x8000)     rowMask |= (1 << 1);  // F1
            if (GetAsyncKeyState(VK_F2) & 0x8000)     rowMask |= (1 << 2);  // F2
            if (GetAsyncKeyState(VK_F3) & 0x8000)     rowMask |= (1 << 3);  // F3
            if (GetAsyncKeyState(VK_F4) & 0x8000)     rowMask |= (1 << 4);  // F4
            if (GetAsyncKeyState(VK_F5) & 0x8000)     rowMask |= (1 << 5);  // F5
            if (GetAsyncKeyState(VK_F6) & 0x8000)     rowMask |= (1 << 6);  // F6
            if (GetAsyncKeyState(VK_F7) & 0x8000)     rowMask |= (1 << 7);  // F7
            if (GetAsyncKeyState(VK_F8) & 0x8000)     rowMask |= (1 << 8);  // F8
            if (GetAsyncKeyState(VK_F9) & 0x8000)     rowMask |= (1 << 9);  // F9
            if (GetAsyncKeyState(VK_F10) & 0x8000)    rowMask |= (1 << 10); // F10
            if (GetAsyncKeyState(VK_F11) & 0x8000)    rowMask |= (1 << 11); // F11
            if (GetAsyncKeyState(VK_F12) & 0x8000)    rowMask |= (1 << 12); // F12
        }

        // Port 0xFF01: Drugi red - QWERTZ (akcenti, 1-0, ', +, Backspace)
        if (address == 0xFF01) {
            if (GetAsyncKeyState(VK_OEM_3) & 0x8000)     rowMask |= (1 << 0);  // ¸ (akcent/cedilla)
            if (GetAsyncKeyState('1') & 0x8000)          rowMask |= (1 << 1);  // 1
            if (GetAsyncKeyState('2') & 0x8000)          rowMask |= (1 << 2);  // 2
            if (GetAsyncKeyState('3') & 0x8000)          rowMask |= (1 << 3);  // 3
            if (GetAsyncKeyState('4') & 0x8000)          rowMask |= (1 << 4);  // 4
            if (GetAsyncKeyState('5') & 0x8000)          rowMask |= (1 << 5);  // 5
            if (GetAsyncKeyState('6') & 0x8000)          rowMask |= (1 << 6);  // 6
            if (GetAsyncKeyState('7') & 0x8000)          rowMask |= (1 << 7);  // 7
            if (GetAsyncKeyState('8') & 0x8000)          rowMask |= (1 << 8);  // 8
            if (GetAsyncKeyState('9') & 0x8000)          rowMask |= (1 << 9);  // 9
            if (GetAsyncKeyState('0') & 0x8000)          rowMask |= (1 << 10); // 0
            if (GetAsyncKeyState(VK_OEM_MINUS) & 0x8000) rowMask |= (1 << 11); // ' (apostrof)
            if (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000)  rowMask |= (1 << 12); // + (plus)
            if (GetAsyncKeyState(VK_BACK) & 0x8000)      rowMask |= (1 << 13); // Backspace
        }

        // Port 0xFF02: Treći red - QWERTZ (Tab, Q, W, E, R, T, Z, U, I, O, P, Š, Đ, Enter)
        if (address == 0xFF02) {
            if (GetAsyncKeyState(VK_TAB) & 0x8000)    rowMask |= (1 << 0);  // Tab
            if (GetAsyncKeyState('Q') & 0x8000)       rowMask |= (1 << 1);  // Q
            if (GetAsyncKeyState('W') & 0x8000)       rowMask |= (1 << 2);  // W
            if (GetAsyncKeyState('E') & 0x8000)       rowMask |= (1 << 3);  // E
            if (GetAsyncKeyState('R') & 0x8000)       rowMask |= (1 << 4);  // R
            if (GetAsyncKeyState('T') & 0x8000)       rowMask |= (1 << 5);  // T
            if (GetAsyncKeyState('Y') & 0x8000)       rowMask |= (1 << 6);  // Z (QWERTZ - fizička pozicija Y)
            if (GetAsyncKeyState('U') & 0x8000)       rowMask |= (1 << 7);  // U
            if (GetAsyncKeyState('I') & 0x8000)       rowMask |= (1 << 8);  // I
            if (GetAsyncKeyState('O') & 0x8000)       rowMask |= (1 << 9);  // O
            if (GetAsyncKeyState('P') & 0x8000)       rowMask |= (1 << 10); // P
            if (GetAsyncKeyState(VK_OEM_4) & 0x8000)  rowMask |= (1 << 11); // Š
            if (GetAsyncKeyState(VK_OEM_6) & 0x8000)  rowMask |= (1 << 12); // Đ
            if (GetAsyncKeyState(VK_RETURN) & 0x8000) rowMask |= (1 << 13); // Enter
        }

        // Port 0xFF03: Četvrti red - QWERTZ (Caps, A, S, D, F, G, H, J, K, L, Č, Ć, Ž)
        if (address == 0xFF03) {
            if (GetAsyncKeyState(VK_CAPITAL) & 0x8000) rowMask |= (1 << 0);  // Caps Lock
            if (GetAsyncKeyState('A') & 0x8000)        rowMask |= (1 << 1);  // A
            if (GetAsyncKeyState('S') & 0x8000)        rowMask |= (1 << 2);  // S
            if (GetAsyncKeyState('D') & 0x8000)        rowMask |= (1 << 3);  // D
            if (GetAsyncKeyState('F') & 0x8000)        rowMask |= (1 << 4);  // F
            if (GetAsyncKeyState('G') & 0x8000)        rowMask |= (1 << 5);  // G
            if (GetAsyncKeyState('H') & 0x8000)        rowMask |= (1 << 6);  // H
            if (GetAsyncKeyState('J') & 0x8000)        rowMask |= (1 << 7);  // J
            if (GetAsyncKeyState('K') & 0x8000)        rowMask |= (1 << 8);  // K
            if (GetAsyncKeyState('L') & 0x8000)        rowMask |= (1 << 9);  // L
            if (GetAsyncKeyState(VK_OEM_1) & 0x8000)   rowMask |= (1 << 10); // Č
            if (GetAsyncKeyState(VK_OEM_7) & 0x8000)   rowMask |= (1 << 11); // Ć
            if (GetAsyncKeyState(VK_OEM_5) & 0x8000)   rowMask |= (1 << 12); // Ž
        }

        // Port 0xFF04: Peti red - QWERTZ (LShift, <, Y, X, C, V, B, N, M, ,, ., -, RShift)
        if (address == 0xFF04) {
            if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)     rowMask |= (1 << 0);  // Left Shift
            if (GetAsyncKeyState(VK_OEM_102) & 0x8000)    rowMask |= (1 << 1);  // < (tipka pored LShift)
            if (GetAsyncKeyState('Z') & 0x8000)           rowMask |= (1 << 2);  // Y (QWERTZ - fizička pozicija Z)
            if (GetAsyncKeyState('X') & 0x8000)           rowMask |= (1 << 3);  // X
            if (GetAsyncKeyState('C') & 0x8000)           rowMask |= (1 << 4);  // C
            if (GetAsyncKeyState('V') & 0x8000)           rowMask |= (1 << 5);  // V
            if (GetAsyncKeyState('B') & 0x8000)           rowMask |= (1 << 6);  // B
            if (GetAsyncKeyState('N') & 0x8000)           rowMask |= (1 << 7);  // N
            if (GetAsyncKeyState('M') & 0x8000)           rowMask |= (1 << 8);  // M
            if (GetAsyncKeyState(VK_OEM_COMMA) & 0x8000)  rowMask |= (1 << 9);  // ,
            if (GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000) rowMask |= (1 << 10); // .
            if (GetAsyncKeyState(VK_OEM_2) & 0x8000)      rowMask |= (1 << 11); // - (crtica)
            if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)     rowMask |= (1 << 12); // Right Shift
        }

        // Port 0xFF05: Šesti red (LCtrl, Win, LAlt, Space, RAlt, RCtrl)
        if (address == 0xFF05) {
            if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) rowMask |= (1 << 0);  // Left Ctrl
            if (GetAsyncKeyState(VK_LWIN) & 0x8000)     rowMask |= (1 << 1);  // Left Win
            if (GetAsyncKeyState(VK_LMENU) & 0x8000)    rowMask |= (1 << 2);  // Left Alt
            if (GetAsyncKeyState(VK_SPACE) & 0x8000)    rowMask |= (1 << 3);  // Space
            if (GetAsyncKeyState(VK_RMENU) & 0x8000)    rowMask |= (1 << 4);  // Right Alt
            if (GetAsyncKeyState(VK_RWIN) & 0x8000)     rowMask |= (1 << 5);  // Right Win
            if (GetAsyncKeyState(VK_APPS) & 0x8000)     rowMask |= (1 << 6);  // Menu key
            if (GetAsyncKeyState(VK_RCONTROL) & 0x8000) rowMask |= (1 << 7);  // Right Ctrl
        }

        return rowMask;
    }

    // Alternativni ASCII unos za Forth (port 0xFFF1)
    if (address == 0xFFF1) {
        if (_kbhit()) return _getch();
        return 0;
    }

    // Disk portovi: 0xFFFC, 0xFFFD, 0xFFFE
    if (address >= 0xFFFC && address <= 0xFFFE) {
        return disk.readPort(address);
    }

    return data[address];
}

// ============================================================================
// PISANJE U MEMORIJU sa I/O mapiranjem
// ============================================================================
void Memory::write(Word address, Word value) {
    // Disk portovi: 0xFFFC, 0xFFFD, 0xFFFE (ne čuvaju se u RAM-u)
    if (address >= 0xFFFC && address <= 0xFFFE) {
        disk.writePort(address, value);
        return;
    }

    // Ostale adrese - upis u RAM
    if (address < MEM_SIZE) {
        data[address] = value;

        // Video mapiranje: Ako softver piše u područje [8192, 10192)
        // Koristi VideoDevice za 80x25 renderiranje
        if (address >= VideoDevice::VIDEO_START_ADDR && 
            address < VideoDevice::VIDEO_START_ADDR + VideoDevice::VIDEO_SIZE) {
            video.writeChar(address, value);
        }
    }
}

// Dodajemo metodu za učitavanje ROM/RAM fajla
bool Memory::loadFromFile(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) return false;
    
    // Učitavamo direktno u naš vektor (maksimalno MEM_SIZE riječi)
    fread(data.data(), sizeof(Word), MEM_SIZE, file);
    fclose(file);
    return true;
}

Word* Memory::getRawPointer(Word address) {
    return &data[address];
}