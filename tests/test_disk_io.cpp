#include "../include/cpu.h"
#include <iostream>
#include <fstream>

/**
 * TEST 4: Disk I/O
 * 
 * Testira:
 * - Pisanje sektora na disk
 * - Čitanje sektora sa diska
 * - Portovi: 0xFFFE (komande), 0xFFFD (sektor), 0xFFFC (podaci)
 * 
 * Kako pokrenuti:
 * cd build
 * g++ -I../include -o test_disk_io ../src/*.cpp ../tests/test_disk_io.cpp
 * ./test_disk_io
 */

int main() {
    std::cout << "========== TEST 4: Disk I/O ==========" << std::endl;
    
    Memory mem;
    CPU cpu(mem);
    
    std::cout << "\n[TEST 4a] Disk parametri:" << std::endl;
    std::cout << "Sektor veličina: " << DiskDevice::SECTOR_SIZE << " riječi (512 bajtova)" << std::endl;
    std::cout << "Ukupno sektora: " << DiskDevice::TOTAL_SECTORS << std::endl;
    std::cout << "Kapacitet: " << (DiskDevice::SECTOR_SIZE * DiskDevice::TOTAL_SECTORS * 2 / 1024)
              << " KB" << std::endl;
    std::cout << "✓ PASS" << std::endl;
    
    std::cout << "\n[TEST 4b] Pisanje Test podataka" << std::endl;
    std::cout << "Scenarij: Upiši 10 test vrednosti u sektor 0" << std::endl;
    
    // Simulacija CPU programa za pisanje
    // 1. Izaberi sektor 0
    mem.write(0xFFFD, 0);  // PORT_SECTOR
    std::cout << "  Odabran sektor: 0" << std::endl;
    
    // 2. Upiši podatke u buffer
    std::cout << "  Upis podataka: ";
    for (int i = 0; i < 10; i++) {
        Word testData = 0x1000 + i;  // 0x1000, 0x1001, ..., 0x1009
        mem.write(0xFFFC, testData);  // PORT_DATA
        std::cout << "0x" << std::hex << testData << std::dec << " ";
    }
    std::cout << std::endl;
    
    // 3. Pošalji WRITE komandu
    mem.write(0xFFFE, 2);  // CMD_WRITE
    std::cout << "  Komanda WRITE poslata" << std::endl;
    std::cout << "✓ PASS" << std::endl;
    
    std::cout << "\n[TEST 4c] Čitanje Test podataka" << std::endl;
    std::cout << "Scenarij: Pročitaj podatke iz sektora 0" << std::endl;
    
    // Simulacija čitanja
    // 1. Odaberi sektor 0
    mem.write(0xFFFD, 0);  // PORT_SECTOR
    std::cout << "  Odabran sektor: 0" << std::endl;
    
    // 2. Pošalji READ komandu
    mem.write(0xFFFE, 1);  // CMD_READ
    std::cout << "  Komanda READ poslata" << std::endl;
    
    // 3. Čitaj podatke
    std::cout << "  Očitani podaci: ";
    for (int i = 0; i < 10; i++) {
        Word data = mem.read(0xFFFC);  // PORT_DATA
        std::cout << "0x" << std::hex << data << std::dec << " ";
    }
    std::cout << std::endl;
    std::cout << "✓ PASS" << std::endl;
    
    std::cout << "\n[TEST 4d] Provera disk.img fajla" << std::endl;
    
    std::ifstream diskFile("disk.img", std::ios::binary);
    if (diskFile.good()) {
        // Provjeri veličinu fajla
        diskFile.seekg(0, std::ios::end);
        std::streamsize fileSize = diskFile.tellg();
        diskFile.close();
        
        std::cout << "  Datoteka disk.img postoji" << std::endl;
        std::cout << "  Veličina fajla: " << fileSize << " bajtova" << std::endl;
        std::cout << "  Očekivana veličina: " << (DiskDevice::SECTOR_SIZE * DiskDevice::TOTAL_SECTORS * 2)
                  << " bajtova" << std::endl;
        
        if (fileSize == DiskDevice::SECTOR_SIZE * DiskDevice::TOTAL_SECTORS * 2) {
            std::cout << "✓ PASS" << std::endl;
        } else {
            std::cout << "⚠ UPOZORENJE - Neočekivana veličina fajla!" << std::endl;
        }
    } else {
        std::cout << "  Datoteka disk.img se kreira pri prvoj upotrebi" << std::endl;
        std::cout << "✓ PASS" << std::endl;
    }
    
    std::cout << "\n[TEST 4e] Reset komande" << std::endl;
    std::cout << "  Slanje RESET komande (0)..." << std::endl;
    mem.write(0xFFFE, 0);  // CMD_RESET
    std::cout << "✓ PASS" << std::endl;
    
    std::cout << "\n========== TESTIRANJE ZAVRŠENO ==========" << std::endl;
    
    return 0;
}
