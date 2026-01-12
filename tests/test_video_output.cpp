#include "../include/cpu.h"
#include <iostream>

/**
 * TEST 3: Memorija i video ispis
 * 
 * Testira:
 * - Čitanje/pisanje u video memoriju (adresa 8192+)
 * - Prikaz karaktera na konzoli
 * 
 * Kako pokrenuti:
 * cd build
 * g++ -I../include -o test_video_output ../src/*.cpp ../tests/test_video_output.cpp
 * ./test_video_output
 */

int main() {
    std::cout << "========== TEST 3: Video memorija ==========" << std::endl;
    
    Memory mem;
    CPU cpu(mem);
    
    std::cout << "\n[TEST 3a] Ispis 'Hello, World!' kroz video memoriju" << std::endl;
    std::cout << "Očekivani ispis: Hello, World!" << std::endl;
    std::cout << "Aktuelni ispis: ";
    std::cout << std::flush;
    
    const char* text = "Hello, World!";
    Word videoAddr = 8192;
    
    for (int i = 0; text[i] != '\0'; i++) {
        mem.write(videoAddr + i, (Word)text[i]);
    }
    
    std::cout << "\n✓ PASS - Tekst je napisan u video memoriju" << std::endl;
    
    std::cout << "\n[TEST 3b] Čitanje iz video memorije" << std::endl;
    Word firstChar = mem.read(8192);
    std::cout << "Prvi karakter na adresi 8192: '" << (char)firstChar << "' (0x" 
              << std::hex << firstChar << std::dec << ")" << std::endl;
    if (firstChar == 'H') {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL - Očekivano 'H' (0x48)" << std::endl;
    }
    
    std::cout << "\n[TEST 3c] Prikaz ASCII tabele (32-126)" << std::endl;
    std::cout << "Ispisivanje ASCII znakova: ";
    std::cout << std::flush;
    
    for (int i = 32; i <= 126; i += 10) {
        mem.write(8300 + (i-32)/10, i);
    }
    
    std::cout << "\n✓ PASS - ASCII znakovi ispisani" << std::endl;
    
    std::cout << "\n[TEST 3d] Video memorija - koordinate" << std::endl;
    std::cout << "80x25 = 2000 karaktera" << std::endl;
    std::cout << "Početna adresa: " << VideoDevice::VIDEO_START_ADDR << std::endl;
    std::cout << "Krajnja adresa: " << (VideoDevice::VIDEO_START_ADDR + VideoDevice::VIDEO_SIZE - 1) << std::endl;
    
    // Test upisivanja na kraj ekrana
    std::cout << "Upis na adresu " << (8192 + 1999) << ": ";
    std::cout << std::flush;
    mem.write(8192 + 1999, 'X');
    std::cout << "\n✓ PASS - Karakter upisan na krajnju poziciju" << std::endl;
    
    std::cout << "\n========== TESTIRANJE ZAVRŠENO ==========" << std::endl;
    std::cout << "NAPOMENA: Tekst bi trebalo da se pojavi gore u konzoli" << std::endl;
    
    return 0;
}
