#pragma once
#include "common.h"
#include <string>
#include <vector>
#include <fstream>

class DiskDevice {
public:
    // Portovi za disk
    static const Word PORT_CMD = 0xFFFE;
    static const Word PORT_SECTOR = 0xFFFD;
    static const Word PORT_DATA = 0xFFFC;

    // Komande
    static const Word CMD_RESET = 0;
    static const Word CMD_READ = 1;
    static const Word CMD_WRITE = 2;

    // Konfiguracija diska
    static const int SECTOR_SIZE = 256;      // 256 riječi po sektoru (512 bajtova)
    static const int TOTAL_SECTORS = 256;    // 256 sektora = 128KB disk

private:
    std::string diskFileName;                // Ime fajla za emulaciju diska
    Word currentSector;                      // Trenutno odabrani sektor
    std::vector<Word> sectorBuffer;          // Buffer za jedan sektor (256 riječi)
    int bufferIndex;                         // Pozicija u bufferu za čitanje/pisanje
    bool sectorLoaded;                       // Da li je sektor učitan u buffer

    // Pomoćne metode
    void loadSector();                       // Učitaj sektor iz fajla u buffer
    void saveSector();                       // Spremi buffer u fajl
    void ensureDiskFileExists();             // Kreiraj disk fajl ako ne postoji

public:
    DiskDevice();
    DiskDevice(const std::string& filename);
    
    void writePort(Word port, Word value);   // CPU piše na port
    Word readPort(Word port);                // CPU čita sa porta
    
    void reset();                            // Resetuj disk kontroler
};