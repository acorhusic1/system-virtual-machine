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
       // std::cerr << "[VIDEO] Greska: Ne mogu dobiti handle konzole!" << std::endl;
        return;
    }
    
    // Postavi velicinu prozora konzole na 80x25
    SMALL_RECT windowSize = {0, 0, COLS - 1, ROWS - 1};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    
    // Postavi velicinu buffera na 80x25
    COORD bufferSize = {COLS, ROWS};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    
    // Sakrij kursor (opciono)
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    // Obrisi ekran na pocetku
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
    
    // Inicijaliziraj ako nije vec
    if (!initialized) {
        init();
    }
    
    // Izracunaj poziciju
    int row = addressToRow(address);
    int col = addressToCol(address);
    
    // Postavi kursor na tu poziciju
    setCursor(row, col);
    
    // Ispisi karakter DIREKTNO u konzolu sa std::cout
    if (value >= 32 && value < 127) {
        // Printable ASCII
        std::cout << (char)value << std::flush;
    } else if (value == 0 || value == 32) {
        // Prazno (space)
        std::cout << ' ' << std::flush;
    }
    // Line feed i Carriage return se ignorišu u grid modu
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
    
    // Vrati kursor na pocetak
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