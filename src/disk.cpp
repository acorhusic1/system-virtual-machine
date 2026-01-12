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
    
    // Inicijaliziraj buffer za jedan sektor (256 rijeci)
    sectorBuffer.resize(SECTOR_SIZE, 0);
    
    // Osiguraj da disk fajl postoji
    ensureDiskFileExists();
}

// ============================================================================
// RESET - Vraca kontroler u pocetno stanje
// ============================================================================

void DiskDevice::reset() {
    currentSector = 0;
    bufferIndex = 0;
    sectorLoaded = false;
    std::fill(sectorBuffer.begin(), sectorBuffer.end(), 0);
}

// ============================================================================
// WRITE PORT - CPU pise na disk portove
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
                    // Ucitaj odabrani sektor u buffer
                    loadSector();
                    bufferIndex = 0;  // Resetuj index za citanje
                    break;
                    
                case CMD_WRITE:
                    // Spremi buffer na disk
                    saveSector();
                    bufferIndex = 0;  // Resetuj index za sljedecu operaciju
                    break;
                    
                default:
                    // Nepoznata komanda: tiho ignoriraj (moze biti normalno pri inicijalizaciji)
                    break;
            }
            break;
        
        // PORT 0xFFFD: Izbor sektora
        case PORT_SECTOR:
            if (value < TOTAL_SECTORS) {
                currentSector = value;
                sectorLoaded = false;  // Novi sektor, treba ucitati
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

        default:
            break;
    }
}

// ============================================================================
// READ PORT - CPU cita sa disk portova
// ============================================================================

Word DiskDevice::readPort(Word port) {
    switch (port) {
        
        // PORT 0xFFFC: citanje podataka iz buffera
        case PORT_DATA:
            if (sectorLoaded && bufferIndex < SECTOR_SIZE) {
                return sectorBuffer[bufferIndex++];
            }
            return 0;
        
        // PORT 0xFFFD: Vraca trenutni sektor (za dijagnostiku)
        case PORT_SECTOR:
            return currentSector;
            
        // PORT 0xFFFE: Vraca status (0 = spreman)
        case PORT_CMD:
            return 0;  // Uvijek spreman (nema emulacije kasnjenja)
            
        default:
            return 0;
    }
}

// ============================================================================
// LOAD SECTOR - Ucitaj sektor iz fajla u buffer
// ============================================================================

void DiskDevice::loadSector() {
    std::ifstream file(diskFileName, std::ios::binary);
    
    if (!file) {
        std::cerr << "[DISK] Greska pri otvaranju fajla za citanje!" << std::endl;
        std::fill(sectorBuffer.begin(), sectorBuffer.end(), 0);
        sectorLoaded = true;
        return;
    }
    
    // Izracunaj poziciju u fajlu (sektor * velicina sektora u bajtovima)
    long offset = currentSector * SECTOR_SIZE * sizeof(Word);
    file.seekg(offset, std::ios::beg);
    
    // Procitaj sektor u buffer
    file.read(reinterpret_cast<char*>(sectorBuffer.data()), SECTOR_SIZE * sizeof(Word));
    
    file.close();
    sectorLoaded = true;
}

// ============================================================================
// SAVE SECTOR - Spremi buffer u fajl
// ============================================================================

void DiskDevice::saveSector() {
    // Otvori fajl za citanje i pisanje (mora postojati)
    std::fstream file(diskFileName, std::ios::binary | std::ios::in | std::ios::out);
    
    if (!file) {
        std::cerr << "[DISK] Greska pri otvaranju fajla za pisanje!" << std::endl;
        return;
    }
    
    // Izracunaj poziciju u fajlu
    long offset = currentSector * SECTOR_SIZE * sizeof(Word);
    file.seekp(offset, std::ios::beg);
    
    // Zapisi buffer u fajl
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
        return;  // Fajl vec postoji
    }
    
    // Kreiraj novi prazan disk fajl
    std::ofstream newFile(diskFileName, std::ios::binary);
    if (!newFile) {
        std::cerr << "[DISK] Greska pri kreiranju disk fajla!" << std::endl;
        return;
    }
    
    // Popuni nulama (TOTAL_SECTORS * SECTOR_SIZE * 2 bajtova)
    std::vector<Word> emptyData(TOTAL_SECTORS * SECTOR_SIZE, 0);
    newFile.write(reinterpret_cast<char*>(emptyData.data()), emptyData.size() * sizeof(Word));
    
    newFile.close();
}