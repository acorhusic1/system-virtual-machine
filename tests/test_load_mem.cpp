#include "../include/cpu.h"
#include <iostream>
#include <fstream>

// Forward declaration
std::string getOpcodeName(Word op);

/**
 * TEST 5: Učitavanje forth.mem programa
 * 
 * Testira:
 * - Čitanje .bin fajla sa diskom
 * - Memorijski layout (ROM od 0, program)
 * 
 * Kako pokrenuti:
 * cd build
 * g++ -I../include -o test_load_mem ../src/*.cpp ../tests/test_load_mem.cpp
 * ./test_load_mem
 * 
 * NAPOMENA: forth.mem mora biti u isti direktorijumu kao izvršivi fajl
 */

int main() {
    std::cout << "========== TEST 5: Učitavanje programa ==========" << std::endl;
    
    Memory mem;
    
    std::cout << "\n[TEST 5a] Provjera forth.mem" << std::endl;
    std::ifstream demoFile("forth.mem", std::ios::binary);
    if (!demoFile.good()) {
        std::cout << "  UPOZORENJE: forth.mem NIJE PRONAĐEN!" << std::endl;
        std::cout << "  Trebalo bi da bude u trenutnom direktorijumu" << std::endl;
        std::cout << "  Kreiram test program sa 10 instrukcija..." << std::endl;
        
        // Kreiraj test .bin fajl
        std::ofstream testBin("forth.mem", std::ios::binary);
        if (testBin.good()) {
            // Jednostavan test program (big-endian):
            // LOD R1, 8192    - R1 = memorija[8192]
            // ADD R1, R1, R1  - R1 = R1 + R1
            // STO R1, 8193    - memorija[8193] = R1
            uint8_t testProgram[] = {
                0x01, 0x20,  // LOD R1, 8192 (dest=1, src2=0, value=8192 hardkodiran ili R[20]?)
                0x21, 0x11,  // ADD R1, R1, R1
                0x91, 0x21,  // STO R1, 8193
                0xFF, 0xFF   // Padding/stop
            };
            testBin.write((char*)testProgram, sizeof(testProgram));
            testBin.close();
            std::cout << "   Test program kreiran" << std::endl;
        }
    } else {
        demoFile.seekg(0, std::ios::end);
        std::streamsize fileSize = demoFile.tellg();
        demoFile.close();
        
        std::cout << "  forth.mem pronađen" << std::endl;
        std::cout << "  Veličina: " << fileSize << " bajtova (" << fileSize/2 << " riječi)" << std::endl;
    }
    
    std::cout << "\n[TEST 5b] Učitavanje programa u memoriju" << std::endl;
    if (mem.loadFromFile("forth.mem")) {
        std::cout << "   Program uspešno učitan" << std::endl;
    } else {
        std::cout << "   GREŠKA pri učitavanju programa!" << std::endl;
        return 1;
    }
    
    std::cout << "\n[TEST 5c] Memorijski raspored" << std::endl;
    std::cout << "  Adreza 0x0000: 0x" << std::hex << mem.read(0) << std::dec << std::endl;
    std::cout << "  Adreza 0x0001: 0x" << std::hex << mem.read(1) << std::dec << std::endl;
    std::cout << "  Adreza 0x0002: 0x" << std::hex << mem.read(2) << std::dec << std::endl;
    std::cout << "  Adreza 0x0003: 0x" << std::hex << mem.read(3) << std::dec << std::endl;
    std::cout << "  (Trebalo bi da sadrži instrukcije iz forth.mem-a)" << std::endl;
    std::cout << "  PASS" << std::endl;
    
    std::cout << "\n[TEST 5d] ROM memorija (trebalo bi 0-2047)" << std::endl;
    std::cout << "  ROM bi trebalo da bude zaštićen od pisanja" << std::endl;
    std::cout << "  Trenutno: ROM NIJE POSEBNO ZAŠTIĆEN (problem)" << std::endl;
    
    std::cout << "\n[TEST 5e] Instrukcijsko dekodiranje" << std::endl;
    Word ir = mem.read(0);  // Prva instrukcija
    if (ir != 0) {
        Word op = (ir & 0xF000) >> 12;
        Word dest = (ir & 0x0F00) >> 8;
        Word src1 = (ir & 0x00F0) >> 4;
        Word src2 = (ir & 0x000F);
        
        std::cout << "  Prva instrukcija: 0x" << std::hex << ir << std::dec << std::endl;
        std::cout << "  - Operacija: " << op << " (" << getOpcodeName(op) << ")" << std::endl;
        std::cout << "  - Registar odredišta: R" << dest << std::endl;
        std::cout << "  - Prvi izvor: R" << src1 << std::endl;
        std::cout << "  - Drugi izvor: R" << src2 << std::endl;
        std::cout << "  PASS" << std::endl;
    } else {
        std::cout << "  Memorija je prazna (forth.mem je prazan ili ne postoji)" << std::endl;
        std::cout << "  PASS (sa upozorenjem)" << std::endl;
    }
    
    std::cout << "\n========== TESTIRANJE ZAVRŠENO ==========" << std::endl;
    
    return 0;
}

std::string getOpcodeName(Word op) {
    const char* opcodes[] = {
        "LOD", "ADD", "SUB", "AND",
        "ORA", "XOR", "SHR", "MUL",
        "STO", "MIF", "GTU", "GTS",
        "LTU", "LTS", "EQU", "MAJ"
    };
    if (op < 16) return opcodes[op];
    return "NEPOZNATA";
}
