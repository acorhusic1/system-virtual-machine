#pragma once
#include "common.h"
#include "memory.h"

class CPU {
public:
    static const Word INTERRUPT_VECTOR = 4; // Standardna adresa za tajmer interapt u SVEU16
    bool interruptsEnabled = true;
    void handleInterrupt(); // Metoda koja vrši skok na ISR
    
    // SVEU16 ima 16 registara (R0-R15)
    // R15 se koristi kao Program Counter (PC)
    Word R[16];
    
    // FLAGS i SP možemo zadržati za specifične operacije, ali Forth koristi R2 za Data Stack i R3 za Return Stack
    Word FLAGS;

    Memory& memory;
    bool running;

    CPU(Memory& mem);

    void reset();
    void run(); 
};