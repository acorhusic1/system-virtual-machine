#include "disk.h"
#include <iostream>

// ============================================================================
// KONSTRUKTORI
// ============================================================================

DiskDevice::DiskDevice() : DiskDevice("disk.img") {
    // Delegira na drugi konstruktor sa default imenom fajla
}

DiskDevice::DiskDevice(const std::string& filename) 
    : diskFileName(filename), currentSector(0), bufferIndex(0), sectorLoaded(false) {
    
    // Inicijaliziraj buffer za jedan sektor (256 riječi)
    sectorBuffer.resize(SECTOR_SIZE, 0);
    
    // Osiguraj da disk fajl postoji
    ensureDiskFileExists();
    
    std::cout << "[DISK] Inicijaliziran: " << diskFileName << std::endl;
}

// ============================================================================
// RESET - Vraća kontroler u početno stanje
// ============================================================================

void DiskDevice::reset() {
    currentSector = 0;
    bufferIndex = 0;
    sectorLoaded = false;
    std::fill(sectorBuffer.begin(), sectorBuffer.end(), 0);
    std::cout << "[DISK] Reset" << std::endl;
}

// ============================================================================
// WRITE PORT - CPU piše na disk portove
// ============================================================================

void DiskDevice::writePort(Word port, Word value) {
    switch (port) {
        
        // PORT 0xFFFE: Komandni port
        case PORT_CMD:
            switch (value) {
                case CMD_RESET:
                    reset();
                    break;
                    
                case CMD_READ:
                    // Učitaj odabrani sektor u buffer
                    loadSector();
                    bufferIndex = 0;  // Resetuj index za čitanje
                    std::cout << "[DISK] READ sektor " << currentSector << std::endl;
                    break;
                    
                case CMD_WRITE:
                    // Spremi buffer na disk
                    saveSector();
                    bufferIndex = 0;  // Resetuj index za sljedeću operaciju
                    std::cout << "[DISK] WRITE sektor " << currentSector << std::endl;
                    break;
                    
                default:
                    std::cerr << "[DISK] Nepoznata komanda: " << value << std::endl;
            }
            break;
        
        // PORT 0xFFFD: Izbor sektora
        case PORT_SECTOR:
            if (value < TOTAL_SECTORS) {
                currentSector = value;
                sectorLoaded = false;  // Novi sektor, treba učitati
                bufferIndex = 0;
            } else {
                std::cerr << "[DISK] Neispravan sektor: " << value << std::endl;
            }
            break;
        
        // PORT 0xFFFC: Upis podataka u buffer
        case PORT_DATA:
            if (bufferIndex < SECTOR_SIZE) {
                sectorBuffer[bufferIndex++] = value;
            }
            break;
    }
}

// ============================================================================
// READ PORT - CPU čita sa disk portova
// ============================================================================

Word DiskDevice::readPort(Word port) {
    switch (port) {
        
        // PORT 0xFFFC: Čitanje podataka iz buffera
        case PORT_DATA:
            if (sectorLoaded && bufferIndex < SECTOR_SIZE) {
                return sectorBuffer[bufferIndex++];
            }
            return 0;
        
        // PORT 0xFFFD: Vraća trenutni sektor (za dijagnostiku)
        case PORT_SECTOR:
            return currentSector;
            
        // PORT 0xFFFE: Vraća status (0 = spreman)
        case PORT_CMD:
            return 0;  // Uvijek spreman (nema emulacije kašnjenja)
            
        default:
            return 0;
    }
}

// ============================================================================
// LOAD SECTOR - Učitaj sektor iz fajla u buffer
// ============================================================================

void DiskDevice::loadSector() {
    std::ifstream file(diskFileName, std::ios::binary);
    
    if (!file) {
        std::cerr << "[DISK] Greska pri otvaranju fajla za citanje!" << std::endl;
        std::fill(sectorBuffer.begin(), sectorBuffer.end(), 0);
        sectorLoaded = true;
        return;
    }
    
    // Izračunaj poziciju u fajlu (sektor * veličina sektora u bajtovima)
    long offset = currentSector * SECTOR_SIZE * sizeof(Word);
    file.seekg(offset, std::ios::beg);
    
    // Pročitaj sektor u buffer
    file.read(reinterpret_cast<char*>(sectorBuffer.data()), SECTOR_SIZE * sizeof(Word));
    
    file.close();
    sectorLoaded = true;
}

// ============================================================================
// SAVE SECTOR - Spremi buffer u fajl
// ============================================================================

void DiskDevice::saveSector() {
    // Otvori fajl za čitanje i pisanje (mora postojati)
    std::fstream file(diskFileName, std::ios::binary | std::ios::in | std::ios::out);
    
    if (!file) {
        std::cerr << "[DISK] Greska pri otvaranju fajla za pisanje!" << std::endl;
        return;
    }
    
    // Izračunaj poziciju u fajlu
    long offset = currentSector * SECTOR_SIZE * sizeof(Word);
    file.seekp(offset, std::ios::beg);
    
    // Zapiši buffer u fajl
    file.write(reinterpret_cast<char*>(sectorBuffer.data()), SECTOR_SIZE * sizeof(Word));
    
    file.close();
}

// ============================================================================
// ENSURE DISK FILE EXISTS - Kreiraj prazan disk fajl ako ne postoji
// ============================================================================

void DiskDevice::ensureDiskFileExists() {
    // Provjeri da li fajl postoji
    std::ifstream checkFile(diskFileName, std::ios::binary);
    if (checkFile.good()) {
        checkFile.close();
        return;  // Fajl već postoji
    }
    
    // Kreiraj novi prazan disk fajl
    std::cout << "[DISK] Kreiram novi disk fajl: " << diskFileName << std::endl;
    
    std::ofstream newFile(diskFileName, std::ios::binary);
    if (!newFile) {
        std::cerr << "[DISK] Greska pri kreiranju disk fajla!" << std::endl;
        return;
    }
    
    // Popuni nulama (TOTAL_SECTORS * SECTOR_SIZE * 2 bajtova)
    std::vector<Word> emptyData(TOTAL_SECTORS * SECTOR_SIZE, 0);
    newFile.write(reinterpret_cast<char*>(emptyData.data()), emptyData.size() * sizeof(Word));
    
    newFile.close();
    std::cout << "[DISK] Kreiran disk velicine " << (TOTAL_SECTORS * SECTOR_SIZE * 2 / 1024) << " KB" << std::endl;
}