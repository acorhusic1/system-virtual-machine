#include "keyboard.h"
#include <conio.h>
#include <iostream>

Keyboard::Keyboard(Memory& memory) : memory(memory), cursor(VideoDevice::VIDEO_START_ADDR) {}

void Keyboard::processInput() {
    // NE-BLOCKING pristup: Keyboard input se čita kroz GetAsyncKeyState()
    // u Memory::read() za portove 0xFF00-0xFF05
    // 
    // Ovaj kod VIŠE NE TREBA jer FORTH direktno čita tastaturne portove!
    // Ostavljeno za kompatibilnost, ali NE radi ništa osim čitanja iz konzole
    
    // Možemo ovdje dodati dodatnu funkcionalnost za debugging ako želimo
    if (_kbhit()) {
        char key = _getch();
        // Ne obrađuj ništa - FORTH će sam čitati tastaturne portove
    }
}