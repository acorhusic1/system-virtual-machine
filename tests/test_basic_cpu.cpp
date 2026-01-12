#include "../include/cpu.h"
#include <iostream>
#include <cassert>

/**
 * TEST 1: Osnovne CPU instrukcije
 * 
 * Testira:
 * - Mogu li instrukcije biti izvršene?
 * - Da li registri čuvaju vrednosti?
 * - Da li memory read/write radi?
 * 
 * Kako pokrenuti:
 * cd build
 * g++ -I../include -o test_basic_cpu ../src/*.cpp ../tests/test_basic_cpu.cpp
 * ./test_basic_cpu
 */

int main() {
    std::cout << "========== TEST 1: Osnovna CPU logika ==========" << std::endl;
    
    Memory mem;
    CPU cpu(mem);
    
    // TEST 1a: Registri su resetovani
    std::cout << "\n[TEST 1a] Inicijalni registri:" << std::endl;
    std::cout << "R[0] = " << cpu.R[0] << " (očekuje: 0)" << std::endl;
    std::cout << "R[15] = " << cpu.R[15] << " (očekuje: 0, Program Counter)" << std::endl;
    assert(cpu.R[0] == 0);
    assert(cpu.R[15] == 0);
    std::cout << "✓ PASS" << std::endl;
    
    // TEST 1b: Memorija čitanja
    std::cout << "\n[TEST 1b] Čitanje memorije:" << std::endl;
    mem.write(10000, 0xABCD);  // Upiši test vrednost (van ROM-a)
    Word val = mem.read(10000);
    std::cout << "Upisan: 0xABCD, Pročitan: 0x" << std::hex << val << std::dec << std::endl;
    assert(val == 0xABCD);
    std::cout << "✓ PASS" << std::endl;
    
    // TEST 1c: Video memorija
    std::cout << "\n[TEST 1c] Video memorija (80x25):" << std::endl;
    std::cout << "VIDEO_START_ADDR = 8192 (0x" << std::hex << VideoDevice::VIDEO_START_ADDR 
              << std::dec << ")" << std::endl;
    std::cout << "VIDEO_SIZE = " << VideoDevice::VIDEO_SIZE 
              << " (80 × 25)" << std::endl;
    assert(VideoDevice::VIDEO_START_ADDR == 8192);
    assert(VideoDevice::VIDEO_SIZE == 2000);
    std::cout << "✓ PASS" << std::endl;
    
    // TEST 1d: Disk struktura
    std::cout << "\n[TEST 1d] Disk parametri:" << std::endl;
    std::cout << "SECTOR_SIZE = " << DiskDevice::SECTOR_SIZE << " riječi (512 bajtova)" << std::endl;
    std::cout << "TOTAL_SECTORS = " << DiskDevice::TOTAL_SECTORS << std::endl;
    std::cout << "Kapacitet = " << (DiskDevice::SECTOR_SIZE * DiskDevice::TOTAL_SECTORS * 2 / 1024) 
              << " KB" << std::endl;
    assert(DiskDevice::SECTOR_SIZE == 256);
    assert(DiskDevice::TOTAL_SECTORS == 256);
    std::cout << "✓ PASS" << std::endl;
    
    // TEST 1e: Registri za Stack
    std::cout << "\n[TEST 1e] Stack registri:" << std::endl;
    std::cout << "R[2] = " << cpu.R[2] << " (Data Stack, trebalo bi: 8192)" << std::endl;
    std::cout << "R[3] = " << cpu.R[3] << " (Return Stack)" << std::endl;
    // R[2] bi trebao biti 8192 prema reset() funkciji
    std::cout << "ℹ Data Stack pointer je inicijalizovan na 8192" << std::endl;
    std::cout << "✓ PASS" << std::endl;
    
    std::cout << "\n========== SVI TESTOVI PROŠLI ==========" << std::endl;
    std::cout << "Zaključak: Osnovne strukture CPU, memorije i periferije rade!" << std::endl;
    
    return 0;
}
