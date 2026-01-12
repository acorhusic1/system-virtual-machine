#include "cpu.h"
#include "keyboard.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <windows.h>

int main() {
    // Postavi konzolu za UTF-8
    SetConsoleOutputCP(CP_UTF8);
    
    Memory mem;
    
    // Učitaj forth.mem
    std::ifstream forth_file;
    const char* paths[] = {
        "forth.mem",
        "./forth.mem",
        "../forth.mem",
        "build/forth.mem",
        "./build/forth.mem",
        "../build/forth.mem"
    };
    
    bool opened = false;
    for (const char* path : paths) {
        forth_file.open(path, std::ios::binary);
        if (forth_file.is_open()) {
            std::cout << "Učitavam forth.mem sa: " << path << std::endl;
            opened = true;
            break;
        }
    }
    
    if (!opened) {
        std::cerr << "Greška: forth.mem nije pronađen!" << std::endl;
        std::cerr << "Pokušani putevi:" << std::endl;
        for (const char* path : paths) {
            std::cerr << "  - " << path << std::endl;
        }
        return 1;
    }
    
    // Učitaj forth.mem direktno u memoriju (zaobilazi ROM zaštitu)
    // NAPOMENA: forth.mem iz asemblera je u LITTLE-ENDIAN formatu!
    Word* raw_mem = mem.getRawPointer(0);
    unsigned short address = 0;
    
    // Čitaj bajt po bajt i sastavi riječi (little-endian)
    unsigned char byte_buffer[2];
    while (forth_file.read(reinterpret_cast<char*>(byte_buffer), 2) && address < 65536) {
        // Little-endian: nizak bajt prvi, visok bajt drugi
        raw_mem[address] = byte_buffer[0] | (byte_buffer[1] << 8);
        address++;
    }
    forth_file.close();
    
    std::cout << "forth.mem učitan (" << address << " riječi)" << std::endl;
    
    // DEBUG: Ispiši prve instrukcije
    std::cout << "\n=== DEBUG: Prve instrukcije u memoriji ===" << std::endl;
    for (int i = 0; i < 20; i++) {
        Word instr = raw_mem[i];
        Word op = (instr >> 12) & 0xF;
        Word dest = (instr >> 8) & 0xF;
        Word src1 = (instr >> 4) & 0xF;
        Word src2 = instr & 0xF;
        std::cout << "0x" << std::hex << i << ": 0x" << instr << std::dec;
        std::cout << " (op=" << op << " dest=R" << dest << " src1=R" << src1 << " src2=R" << src2 << ")" << std::endl;
    }
    std::cout << "==========================================\n" << std::endl;
    std::cout << "\n=== SVEU16 Sistemska Virtualna Mašina ===" << std::endl;
    std::cout << "Frekvencija: 1 MHz" << std::endl;
    std::cout << "Interapti: Svakih 20ms" << std::endl;
    std::cout << "Video memorija: 80x25 @ 0x2000" << std::endl;
    std::cout << "Tastatura: Portovi 0xFF00-0xFF05" << std::endl;
    std::cout << "Disk: Portovi 0xFFFC-0xFFFE" << std::endl;
    std::cout << "=========================================\n" << std::endl;
    
    // Inicijaliziraj CPU i tastaturu
    CPU cpu(mem);
    Keyboard keyboard(mem);
    
    std::cout << "FORTH pokrenut. Čeka se inicijalizacija..." << std::endl;
    std::cout << "Možete početi pisati komande nakon poruke 'ok'.\n" << std::endl;
    
    // Pokreni CPU u posebnoj niti
    std::thread cpuThread([&cpu]() {
        cpu.run();
    });
    
    // Glavna petlja - samo čekaj (tastatura se čita kroz GetAsyncKeyState)
    while (cpu.running) {
        keyboard.processInput();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    cpuThread.join();
    
    std::cout << "\n\nSVEU16 zaustavljanje." << std::endl;
    return 0;
}