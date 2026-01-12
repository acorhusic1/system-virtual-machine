#pragma once
#include "common.h"
#include <windows.h>

class VideoDevice {
public:
    // Video memorija pocinje na adresi 8192
    static const Word VIDEO_START_ADDR = 8192;
    static const int ROWS = 25;
    static const int COLS = 80;
    static const int VIDEO_SIZE = ROWS * COLS;  // 2000 karaktera

private:
    HANDLE hConsole;           // Handle za Windows konzolu
    bool initialized;          // Da li je konzola inicijalizirana

public:
    VideoDevice();
    ~VideoDevice();
    
    // Inicijalizacija konzole (poziva se jednom na pocetku)
    void init();
    
    // Postavi kursor na poziciju (red, kolona)
    void setCursor(int row, int col);
    
    // Ispisi karakter na poziciju (izracunatu iz adrese)
    void writeChar(Word address, Word value);
    
    // Obrisi ekran
    void clearScreen();
    
    // Pomocne funkcije
    static int addressToRow(Word address);
    static int addressToCol(Word address);
};