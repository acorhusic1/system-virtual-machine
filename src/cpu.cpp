#include "cpu.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;

CPU::CPU(Memory& mem) : memory(mem) {
    reset();
}

void CPU::reset() {
    for (int i = 0; i < 16; i++) R[i] = 0;
    R[0] = 0;       // R0 = 0 (uvijek)
    R[1] = 1;       // R1 = 1 (konstanta)
    R[15] = 0;      // PC na pocetak ROM-a
    R[2] = 0xAEB0;  // Data Stack Pointer (ESPP iz forth.asm)
    R[3] = 0xAE70;  // Return Stack Pointer (ERP iz forth.asm)
    R[4] = 0;       // IP (Interpreter Pointer)
    R[9] = 0x50;    // NEXT1 rutina (iz forth.asm)
    running = true;
}

void CPU::run() {
    // Direktno poravnato interpretiranje (Computed Goto)
    static void* dispatch_table[] = {
        &&op_lod, &&op_add, &&op_sub, &&op_and,
        &&op_ora, &&op_xor, &&op_shr, &&op_mul,
        &&op_sto, &&op_mif, &&op_gtu, &&op_gts,
        &&op_ltu, &&op_lts, &&op_equ, &&op_maj
    };

    auto last_interrupt = steady_clock::now();
    unsigned long instruction_count = 0;
    
next_instruction:
    if (!running) return;

    // SIMULACIJA 1MHz: Ne cekaj nakon SVAKE instrukcije
    // Umjesto toga, cekaj svakih 1000 instrukcija da se simulira 1ms
    instruction_count++;
    if (instruction_count >= 1000) {
        instruction_count = 0;
        std::this_thread::sleep_for(std::chrono::microseconds(100)); // Mala pauza za drugim nitima
    }

    // INTERAPT: Svakih 20ms (Specifikacija)
    auto now = steady_clock::now();
    if (duration_cast<milliseconds>(now - last_interrupt).count() >= 20) {
        last_interrupt = now;
        Word vector = memory.read(0x0004);
        if (vector != 0) {
            R[3]--; // Push na return stack
            memory.write(R[3], R[15]);
            R[15] = vector;
            goto next_instruction;
        }
    }

    // FETCH
    Word ir = memory.read(R[15]++);
    Word op = (ir & 0xF000) >> 12;
    Word dest = (ir & 0x0F00) >> 8;
    Word src1 = (ir & 0x00F0) >> 4;
    Word src2 = (ir & 0x000F);

    // DEBUG: Ispisi prvih 100 instrukcija
    static int debug_count = 0;
    if (debug_count < 100) {
        
        std::cerr << "PC=" << std::hex << (R[15]-1) << " IR=" << ir 
                  << " op=" << std::dec << op << " R2=" << std::hex << R[2] 
                  << " R3=" << R[3] << " R4=" << R[4] << std::dec << std::endl;
        
        debug_count++;
    }

    // Computed goto dispatch
    goto *dispatch_table[op];

    // INSTRUKCIJE
    op_lod: 
        R[dest] = memory.read(R[src2]); 
        if(src2==15 && dest!=15) R[15]++; 
        goto next_instruction;
        
    op_add: 
        R[dest] = R[src1] + R[src2]; 
        goto next_instruction;
        
    op_sub: 
        R[dest] = R[src1] - R[src2]; 
        goto next_instruction;
        
    op_and: 
        R[dest] = R[src1] & R[src2]; 
        goto next_instruction;
        
    op_ora: 
        R[dest] = R[src1] | R[src2]; 
        goto next_instruction;
        
    op_xor: 
        R[dest] = R[src1] ^ R[src2]; 
        goto next_instruction;
        
    op_shr: {
        Word n = R[src2] & 0xF;
        Word mode = (R[src2] >> 4) & 0x3;
        if(mode==0) R[dest] = (int16_t)R[src1] >> n;        // Aritmeticki
        else if(mode==1) R[dest] = R[src1] >> n;            // Logicki desno
        else if(mode==2) R[dest] = R[src1] << n;            // Logicki lijevo
        else R[dest] = (R[src1] << n) | (R[src1] >> (16-n)); // Rotacija
        goto next_instruction;
    }
    
    op_mul: 
        R[dest] = R[src1] * R[src2]; 
        goto next_instruction;
        
    op_sto: 
        R[dest] = R[src1];
        memory.write(R[src2], R[src1]); 
        if(src2==15) R[15]++; 
        goto next_instruction;
        
    op_mif: 

        if(R[src1] != 0) R[dest] = R[src2]; 
        goto next_instruction;
        
    op_gtu: 
        R[dest] = (R[src1] > R[src2]) ? 1 : 0; 
        goto next_instruction;
        
    op_gts: 
        R[dest] = ((int16_t)R[src1] > (int16_t)R[src2]) ? 1 : 0; 
        goto next_instruction;
        
    op_ltu: 
        R[dest] = (R[src1] < R[src2]) ? 1 : 0; 
        goto next_instruction;
        
    op_lts: 
        R[dest] = ((int16_t)R[src1] < (int16_t)R[src2]) ? 1 : 0; 
        goto next_instruction;
        
    op_equ: 
        R[dest] = (R[src1] == R[src2]) ? 1 : 0; 
        goto next_instruction;
        
    op_maj: 
        R[dest] = R[src1]; 
        R[15] = R[src2]; 
        goto next_instruction;
}

void CPU::handleInterrupt() {
    // 1. Spasi trenutni PC (R15) na Return Stack (R3)
    // Prvo smanjimo R3, pa upisemo PC
    R[3]--; 
    memory.write(R[3], R[15]);

    // 2. Skoci na Interrupt Vector
    // citamo adresu iz ROM-a sa lokacije 4
    R[15] = memory.read(INTERRUPT_VECTOR);

    interruptsEnabled = true;
    
    //std::cout << "[INTERRUPT] Skok na: " << R[15] << std::endl;
}