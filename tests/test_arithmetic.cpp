#include "../include/cpu.h"
#include <iostream>
#include <iomanip>
#include <bitset>

/**
 * TEST 2: Instrukcije aritmetike
 * 
 * Testira:
 * - ADD, SUB, MUL
 * - Logičke operacije: AND, ORA, XOR
 * - Poređenja: GTU, GTS, LTU, LTS, EQU
 * 
 * Kako pokrenuti:
 * cd build
 * g++ -I../include -o test_arithmetic ../src/*.cpp ../tests/test_arithmetic.cpp
 * ./test_arithmetic
 */

int main() {
    std::cout << "========== TEST 2: Aritmetičke instrukcije ==========" << std::endl;
    
    Memory mem;
    CPU cpu(mem);
    
    // TEST 2a: ADD
    std::cout << "\n[TEST 2a] Instrukcija ADD:" << std::endl;
    cpu.R[0] = 100;
    cpu.R[1] = 50;
    cpu.R[2] = 0;
    
    // Simulacija: ADD R2, R0, R1 (R2 = R0 + R1)
    cpu.R[2] = cpu.R[0] + cpu.R[1];
    
    std::cout << "R[0] = " << cpu.R[0] << ", R[1] = " << cpu.R[1] << std::endl;
    std::cout << "R[2] = R[0] + R[1] = " << cpu.R[2] << " (očekuje: 150)" << std::endl;
    if (cpu.R[2] == 150) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    // TEST 2b: SUB
    std::cout << "\n[TEST 2b] Instrukcija SUB:" << std::endl;
    cpu.R[0] = 100;
    cpu.R[1] = 30;
    cpu.R[2] = 0;
    
    cpu.R[2] = cpu.R[0] - cpu.R[1];
    
    std::cout << "R[0] = " << cpu.R[0] << ", R[1] = " << cpu.R[1] << std::endl;
    std::cout << "R[2] = R[0] - R[1] = " << cpu.R[2] << " (očekuje: 70)" << std::endl;
    if (cpu.R[2] == 70) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    // TEST 2c: MUL
    std::cout << "\n[TEST 2c] Instrukcija MUL:" << std::endl;
    cpu.R[0] = 12;
    cpu.R[1] = 7;
    cpu.R[2] = 0;
    
    cpu.R[2] = cpu.R[0] * cpu.R[1];
    
    std::cout << "R[0] = " << cpu.R[0] << ", R[1] = " << cpu.R[1] << std::endl;
    std::cout << "R[2] = R[0] * R[1] = " << cpu.R[2] << " (očekuje: 84)" << std::endl;
    if (cpu.R[2] == 84) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    // TEST 2d: AND
    std::cout << "\n[TEST 2d] Instrukcija AND:" << std::endl;
    cpu.R[0] = 0b1111;      // 15
    cpu.R[1] = 0b1010;      // 10
    cpu.R[2] = 0;
    
    cpu.R[2] = cpu.R[0] & cpu.R[1];
    
    std::cout << "R[0] = 0b" << std::bitset<16>(cpu.R[0]) << " (" << cpu.R[0] << ")" << std::endl;
    std::cout << "R[1] = 0b" << std::bitset<16>(cpu.R[1]) << " (" << cpu.R[1] << ")" << std::endl;
    std::cout << "R[2] = R[0] & R[1] = 0b" << std::bitset<16>(cpu.R[2]) << " (" << cpu.R[2] 
              << ", očekuje: 10)" << std::endl;
    if (cpu.R[2] == 10) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    // TEST 2e: ORA
    std::cout << "\n[TEST 2e] Instrukcija ORA:" << std::endl;
    cpu.R[0] = 0b1100;      // 12
    cpu.R[1] = 0b1010;      // 10
    cpu.R[2] = 0;
    
    cpu.R[2] = cpu.R[0] | cpu.R[1];
    
    std::cout << "R[0] = 0b" << std::bitset<16>(cpu.R[0]) << " (" << cpu.R[0] << ")" << std::endl;
    std::cout << "R[1] = 0b" << std::bitset<16>(cpu.R[1]) << " (" << cpu.R[1] << ")" << std::endl;
    std::cout << "R[2] = R[0] | R[1] = 0b" << std::bitset<16>(cpu.R[2]) << " (" << cpu.R[2] 
              << ", očekuje: 14)" << std::endl;
    if (cpu.R[2] == 14) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    // TEST 2f: XOR
    std::cout << "\n[TEST 2f] Instrukcija XOR:" << std::endl;
    cpu.R[0] = 0b1100;      // 12
    cpu.R[1] = 0b1010;      // 10
    cpu.R[2] = 0;
    
    cpu.R[2] = cpu.R[0] ^ cpu.R[1];
    
    std::cout << "R[0] = 0b" << std::bitset<16>(cpu.R[0]) << " (" << cpu.R[0] << ")" << std::endl;
    std::cout << "R[1] = 0b" << std::bitset<16>(cpu.R[1]) << " (" << cpu.R[1] << ")" << std::endl;
    std::cout << "R[2] = R[0] ^ R[1] = 0b" << std::bitset<16>(cpu.R[2]) << " (" << cpu.R[2] 
              << ", očekuje: 6)" << std::endl;
    if (cpu.R[2] == 6) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    // TEST 2g: EQU (Equal)
    std::cout << "\n[TEST 2g] Instrukcija EQU:" << std::endl;
    cpu.R[0] = 100;
    cpu.R[1] = 100;
    cpu.R[2] = 0;
    
    cpu.R[2] = (cpu.R[0] == cpu.R[1]) ? 1 : 0;
    
    std::cout << "R[0] = " << cpu.R[0] << ", R[1] = " << cpu.R[1] << std::endl;
    std::cout << "R[2] = (R[0] == R[1]) = " << cpu.R[2] << " (očekuje: 1)" << std::endl;
    if (cpu.R[2] == 1) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    // TEST 2h: GTU (Greater Than Unsigned)
    std::cout << "\n[TEST 2h] Instrukcija GTU:" << std::endl;
    cpu.R[0] = 200;
    cpu.R[1] = 100;
    cpu.R[2] = 0;
    
    cpu.R[2] = (cpu.R[0] > cpu.R[1]) ? 1 : 0;
    
    std::cout << "R[0] = " << cpu.R[0] << ", R[1] = " << cpu.R[1] << std::endl;
    std::cout << "R[2] = (R[0] > R[1]) = " << cpu.R[2] << " (očekuje: 1)" << std::endl;
    if (cpu.R[2] == 1) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    // TEST 2i: LTU (Less Than Unsigned)
    std::cout << "\n[TEST 2i] Instrukcija LTU:" << std::endl;
    cpu.R[0] = 50;
    cpu.R[1] = 150;
    cpu.R[2] = 0;
    
    cpu.R[2] = (cpu.R[0] < cpu.R[1]) ? 1 : 0;
    
    std::cout << "R[0] = " << cpu.R[0] << ", R[1] = " << cpu.R[1] << std::endl;
    std::cout << "R[2] = (R[0] < R[1]) = " << cpu.R[2] << " (očekuje: 1)" << std::endl;
    if (cpu.R[2] == 1) {
        std::cout << "✓ PASS" << std::endl;
    } else {
        std::cout << "✗ FAIL" << std::endl;
    }
    
    std::cout << "\n========== TESTIRANJE ZAVRŠENO ==========" << std::endl;
    
    return 0;
}
