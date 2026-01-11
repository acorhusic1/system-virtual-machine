#include "cpu.h"
#include "keyboard.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h> // For _kbhit() and _getch()

// Pomoćne funkcije za tajming
using namespace std::chrono;

CPU::CPU(Memory& mem) : memory(mem) {
    reset();
}

void CPU::reset() {
    for (int i = 0; i < 16; i++) R[i] = 0;
    FLAGS = 0;
    running = true;
    interruptPending = false;
    tickCount = 0;
    // R15 je PC, on kreće od 0 (gdje je ROM)
    R[15] = 0; 
    R[2] = 0x7000;
    R[3] = 0x7F00;  
}

void CPU::run() {
    Keyboard keyboard(memory); // Create a Keyboard instance
    
    static void* dispatch_table[] = {
        &&op_lod, &&op_add, &&op_sub, &&op_and,
        &&op_ora, &&op_xor, &&op_shr, &&op_mul,
        &&op_sto, &&op_mif, &&op_gtu, &&op_gts,
        &&op_ltu, &&op_lts, &&op_equ, &&op_maj
    };

    auto last_interrupt = steady_clock::now();
    auto last_cycle = steady_clock::now();
    // Frekvencija 1 MHz = 1 instrukcija po mikrosekundi
    const nanoseconds cycle_duration(1000); 

next_instruction:
    if (!running) return;

    // Process keyboard input
    keyboard.processInput();

    // --- LOGIKA ZA INTERAPTE (Svakih 20ms) ---
    auto now = steady_clock::now();
    if (duration_cast<milliseconds>(now - last_interrupt).count() >= 20) {
        last_interrupt = now;
        interruptPending = true;
    }

    // --- FREQUENCY LIMITER (1 MHz) ---
    // Čekaj dok ne prođe 1 mikrosekunda od zadnjeg ciklusa
    auto target = last_cycle + cycle_duration;
    std::this_thread::sleep_until(target);
    last_cycle = target;

    if (interruptPending) {
        interruptPending = false;
        tickCount++;
    
        goto next_instruction; // ovo “pojede” jedan ciklus
    }

    // FETCH & DECODE
    Word ir = memory.read(R[15]++);
    Word op = (ir & 0xF000) >> 12;
    Word dest = (ir & 0x0F00) >> 8;
    Word src1 = (ir & 0x00F0) >> 4;
    Word src2 = (ir & 0x000F);

    /*
    // Privremeni ispis za testiranje
    if (R[15] < 100) { 
        std::cout << "PC: 0x" << std::hex << R[15] << " IR: " << ir << " Op: " << (int)op << std::endl;
    }
    */

    goto *dispatch_table[op];

    op_lod: 
        // Ako čitamo sa adrese na koju pokazuje PC, to je konstanta
        R[dest] = memory.read(R[src2]);
        if (src2 == 15 && dest != 15) {
            R[15]++;
        }
        goto next_instruction;
    op_add: R[dest] = R[src1] + R[src2]; goto next_instruction;
    op_sub: R[dest] = R[src1] - R[src2]; goto next_instruction;
    op_and: R[dest] = R[src1] & R[src2]; goto next_instruction;
    op_ora: R[dest] = R[src1] | R[src2]; goto next_instruction;
    op_xor: R[dest] = R[src1] ^ R[src2]; goto next_instruction;
    op_shr: {
        Word n = R[src2] & 0xF;
        Word kind = (R[src2] & 0x0030) >> 4;
        if (kind == 0) R[dest] = (int16_t)R[src1] >> n;      // Arithmetic
        else if (kind == 1) R[dest] = R[src1] >> n;          // Logical
        else if (kind == 2) R[dest] = R[src1] << n;          // Left
        else R[dest] = (R[src1] << n) | (R[src1] >> (16 - n)); // Rotate
        goto next_instruction;
    }
    op_mul: R[dest] = R[src1] * R[src2]; goto next_instruction;
    op_sto: 
        // Ako pišemo na adresu na koju pokazuje PC, to je direktno adresiranje
        memory.write(R[src2], R[src1]);

        // poravnanje:
        if (src2 == 15) {
            R[15]++; // preskoči "extra word"
        } 

        R[dest] = R[src1]; 
        goto next_instruction;
    op_mif:
        if (R[src1] != 0) R[dest] = src2; 
        goto next_instruction;
    op_gtu: R[dest] = (R[src1] > R[src2]); goto next_instruction;
    op_gts: R[dest] = ((int16_t)R[src1] > (int16_t)R[src2]); goto next_instruction;
    op_ltu: R[dest] = (R[src1] < R[src2]); goto next_instruction;
    op_lts: R[dest] = ((int16_t)R[src1] < (int16_t)R[src2]); goto next_instruction;
    op_equ: R[dest] = (R[src1] == R[src2]); goto next_instruction;
    op_maj: 
        R[dest] = R[src1];
        R[15]   = R[src2]; 
        goto next_instruction;
}

void CPU::handleInterrupt() {
    // 1. Spasi trenutni PC (R15) na Return Stack (R3)
    // Prvo smanjimo R3, pa upišemo PC
    R[3]--; 
    memory.write(R[3], R[15]);

    // 2. Skoči na Interrupt Vector
    // Čitamo adresu iz ROM-a sa lokacije 4
    R[15] = memory.read(INTERRUPT_VECTOR);

    interruptsEnabled = true;
    
    //std::cout << "[INTERRUPT] Skok na: " << R[15] << std::endl;
}