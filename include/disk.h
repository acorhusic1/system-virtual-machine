#ifndef DISK_H
#define DISK_H

#include "common.h"
#include <string>
#include <vector>
#include <fstream>

class DiskDevice {
public:
    DiskDevice();
    DiskDevice(const std::string& filename);
    
    void reset();
    void writePort(Word port, Word value);
    Word readPort(Word port);
    
    // Specifikacija: Portovi
    static const Word PORT_CMD = 0xFFFE;    // Komandni port
    static const Word PORT_SECTOR = 0xFFFD; // Izbor sektora
    static const Word PORT_DATA = 0xFFFC;   // Prenos podataka
    
    // Specifikacija: Komande
    static const Word CMD_RESET = 0;
    static const Word CMD_READ = 1;
    static const Word CMD_WRITE = 2;
    
    // Specifikacija: Disk je 256 sektora, svaki sektor 256 riječi
    static const int SECTOR_SIZE = 256;
    static const int TOTAL_SECTORS = 256;
    
private:
    std::string diskFileName;
    Word currentSector;
    std::vector<Word> sectorBuffer;
    int bufferIndex;
    bool sectorLoaded;
    
    void loadSector();
    void saveSector();
    void ensureDiskFileExists();
};

#endif // DISK_H