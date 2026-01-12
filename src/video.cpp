#include "video.h"
#include <iostream>
#include <windows.h>

VideoDevice::VideoDevice() : hConsole(NULL), initialized(false) {
}

VideoDevice::~VideoDevice() {
    // Konzola se automatski zatvara sa programom
}

void VideoDevice::init() {
    if (initialized) return;
    
    // Dobij handle za standardni output (konzola)
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "[VIDEO] Greška: Ne mogu dobiti handle konzole!" << std::endl;
        return;
    }
    
    // Postavi veličinu prozora konzole na 80x25
    SMALL_RECT windowSize = {0, 0, COLS - 1, ROWS - 1};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    
    // Postavi veličinu buffera na 80x25
    COORD bufferSize = {COLS, ROWS};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    
    // Sakrij kursor (opciono)
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    // Obriši ekran na početku
    clearScreen();
    
    initialized = true;
}

void VideoDevice::setCursor(int row, int col) {
    if (!initialized || hConsole == NULL) return;
    
    // Provjeri granice
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;
    
    COORD position;
    position.X = (SHORT)col;
    position.Y = (SHORT)row;
    
    SetConsoleCursorPosition(hConsole, position);
}

void VideoDevice::writeChar(Word address, Word value) {
    // Provjeri da li je adresa u video memoriji
    if (address < VIDEO_START_ADDR || address >= VIDEO_START_ADDR + VIDEO_SIZE) {
        return;
    }
    
    // Inicijaliziraj ako nije već
    if (!initialized) {
        init();
    }
    
    // DEBUG: Ispiši prvih nekoliko upisa
    static int write_count = 0;
    if (write_count < 50) {
        std::cerr << "[VIDEO] Pisanje na adresu 0x" << std::hex << address 
                  << " vrijednost 0x" << value << " ('" << (char)(value >= 32 && value < 127 ? value : '?') 
                  << "')" << std::dec << std::endl;
        write_count++;
    }
    
    // Izračunaj poziciju
    int row = addressToRow(address);
    int col = addressToCol(address);
    
    // Postavi kursor na tu poziciju
    setCursor(row, col);
    
    // Ispiši karakter DIREKTNO u konzolu
    if (value >= 32 && value < 127) {
        // Printable ASCII
        std::cout << (char)value << std::flush;
    } else if (value == 0 || value == 32) {
        // Prazno (space)
        std::cout << ' ' << std::flush;
    } else if (value == 10) {
        // Line feed - prijeđi u novi red
        std::cout << std::endl;
    } else if (value == 13) {
        // Carriage return - vrati se na početak reda
        std::cout << '\r' << std::flush;
    }
}

void VideoDevice::clearScreen() {
    if (!initialized || hConsole == NULL) return;
    
    COORD topLeft = {0, 0};
    DWORD written;
    DWORD consoleSize = ROWS * COLS;
    
    // Popuni ekran razmacima
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &written);
    
    // Resetuj atribute (boja)
    FillConsoleOutputAttribute(hConsole, 
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,  // Bijela boja
        consoleSize, topLeft, &written);
    
    // Vrati kursor na početak
    SetConsoleCursorPosition(hConsole, topLeft);
}

int VideoDevice::addressToRow(Word address) {
    // Formula: red = (adresa - 8192) / 80
    return (address - VIDEO_START_ADDR) / COLS;
}

int VideoDevice::addressToCol(Word address) {
    // Formula: kolona = (adresa - 8192) % 80
    return (address - VIDEO_START_ADDR) % COLS;
}