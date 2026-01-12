#include "cpu.h"
#include "memory.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

int main() {
    std::cout << "=== TEST HELLO ===" << std::endl;
    
    Memory mem;
    Word* raw = mem.getRawPointer(0);

    system("cls");
    
    // Jednostavan program:
    // 1. Upisi 'H' (0x48) na video memoriju adresu 0x2000
    // 2. Upisi 'E' (0x45) na 0x2001
    // 3. Upisi 'L' (0x4C) na 0x2002
    // 4. Upisi 'L' (0x4C) na 0x2003
    // 5. Upisi 'O' (0x4F) na 0x2004
    // 6. Infinite loop
    
    int pc = 0;
    
    // LOD R5, R15, R15  - ucitaj 'H' u R5 (LOD = 0x0)
    raw[pc++] = 0x05FF;  // op=0, dest=R5, src1=R15, src2=R15
    raw[pc++] = 0x0048;  // data: 'H'
    
    // LOD R6, R15, R15  - ucitaj adresu 0x2000 u R6
    raw[pc++] = 0x06FF;
    raw[pc++] = 0x2000;  // video memory start
    
    // STO R5, R5, R6    - spremi R5 na adresu u R6 (STO = 0x8)
    raw[pc++] = 0x8556;
    
    // LOD R5, R15, R15  - ucitaj 'E'
    raw[pc++] = 0x05FF;
    raw[pc++] = 0x0045;
    
    // ADD R6, R6, R1    - R6 = R6 + 1 (ADD = 0x1)
    raw[pc++] = 0x1661;
    
    // STO R5, R5, R6    - spremi 'E'
    raw[pc++] = 0x8556;
    
    // LOD R5, R15, R15  - ucitaj 'L'
    raw[pc++] = 0x05FF;
    raw[pc++] = 0x004C;
    
    // ADD R6, R6, R1
    raw[pc++] = 0x1661;
    
    // STO R5, R5, R6
    raw[pc++] = 0x8556;
    
    // LOD R5, R15, R15  - jos jedno 'L'
    raw[pc++] = 0x05FF;
    raw[pc++] = 0x004C;
    
    // ADD R6, R6, R1
    raw[pc++] = 0x1661;
    
    // STO R5, R5, R6
    raw[pc++] = 0x8556;
    
    // LOD R5, R15, R15  - 'O'
    raw[pc++] = 0x05FF;
    raw[pc++] = 0x004F;
    
    // ADD R6, R6, R1
    raw[pc++] = 0x1661;
    
    // STO R5, R5, R6
    raw[pc++] = 0x8556;
    
    // Infinite loop: ORA R15, R15, R15 (stay at same address)
    int loop_addr = pc;
    raw[pc++] = 0x4FFF;  // ORA R15, R15, R15
    
    CPU cpu(mem);
    
    // Run CPU for a bit
    std::thread cpuThread([&cpu]() {
        cpu.run();
    });
    
    // Let it run for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cpu.running = false;
    
    cpuThread.join();
    
    // Check video memory
    std::cout << "\n\nVideo memory contents:" << std::endl;
    for (int i = 0; i < 10; i++) {
        Word val = mem.read(0x2000 + i);
        std::cout << "0x" << std::hex << (0x2000 + i) << ": 0x" << val;
        if (val >= 32 && val < 127) {
            std::cout << " ('" << (char)val << "')";
        }
        std::cout << std::dec << std::endl;
    }
    
    return 0;
}