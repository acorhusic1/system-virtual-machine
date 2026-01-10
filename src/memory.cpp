#include "memory.h"
#include <iostream>
#include <conio.h>
#include <windows.h> // Za GetAsyncKeyState

Memory::Memory() {
    data.resize(MEM_SIZE, 0);
}

Word Memory::read(Word address) {
    // Tastatura: Portovi 0xFF00 - 0xFF05
    if (address >= 0xFF00 && address <= 0xFF05) {
        Word rowMask = 0;

        // Primjer mapiranja za Port 0xFF01 (Drugi red: 1, 2, 3...)
        if (address == 0xFF01) {
            if (GetAsyncKeyState('1') & 0x8000) rowMask |= (1 << 1);
            if (GetAsyncKeyState('2') & 0x8000) rowMask |= (1 << 2);
            if (GetAsyncKeyState('3') & 0x8000) rowMask |= (1 << 3);
            // ... dodaj ostale tastere iz tog reda po potrebi
        }
        
        // Primjer mapiranja za Port 0xFF02 (Treći red: Tab, Q, W, E, R, T, Y, U, I, O, P, [, ], Enter)
        if (address == 0xFF02) {
            if (GetAsyncKeyState('Q') & 0x8000) rowMask |= (1 << 1);
            if (GetAsyncKeyState('W') & 0x8000) rowMask |= (1 << 2);
            if (GetAsyncKeyState('E') & 0x8000) rowMask |= (1 << 3);
            if (GetAsyncKeyState('R') & 0x8000) rowMask |= (1 << 4);
            if (GetAsyncKeyState(VK_RETURN) & 0x8000) rowMask |= (1 << 13); // ENTER je bit 13/14 u ovom redu
        }

        // Primjer mapiranja za Port 0xFF05 (Peti red: Space)
        if (address == 0xFF05) {
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) rowMask |= (1 << 4); 
        }

        // Ako se koristi standardni Forth unos, on često traži ASCII
        // pa ćemo dodati i podršku za klasični conio ulaz ako zatreba
        if (address == 0xFFF1) { // Prema tvom prvom kodu
            if (_kbhit()) return _getch();
        }

        
        // TEST - OBRISI KASNIJE
        if (rowMask > 0) {
            // Svaki put kad stisneš taster koji je mapiran, vidjet ćeš ovo u konzoli
            std::cout << "[KBD] Read Addr: " << std::hex << address << " Val: " << rowMask << std::endl;
        }
        

        return rowMask;
    }

    if (address == 0xFFFC) return 0; // Disk status
    return data[address];
}

// Čitanje iz memorije sa specijalnim mapiranim uređajima
/*
Word Memory::read(Word address) {
    // Vrati stanje tastera iz Keyboard klase
    if (address >= 0xFF00 && address <= 0xFF05) {
        return 0;
    }

    // Vrati podatak sa diska
    if (address == 0xFFFC) {
        return 0;
    }

    return data[address];
}
*/



// Stvarna implementacija koja ce se koristiti kasnije
/*
void Memory::write(Word address, Word value) {
    // 1. Upis u osnovni niz (RAM/VRAM)
    if (address < MEM_SIZE) {
        data[address] = value;

        // 2. Video Mapiranje
        if (address >= 8192 && address < 8192 + (80 * 25)) {
             if (value >= 32 && value < 127) {
                 std::cout << (char)value << std::flush;
             }
        }
        
        // 3. Disk Portovi (prema specifikaciji)
        else if (address == 0xFFFE) {
            // Ovdje kasnije pozovi disk.executeCommand(value);
            std::cout << "[Disk] Primljena komanda: " << value << std::endl;
        }
        else if (address == 0xFFFD) {
            // disk.setSector(value);
        }
        else if (address == 0xFFFC) {
            // disk.setData(value);
        }
    }
}
*/



// Privremena implementacija sa ispisom na konzolu
void Memory::write(Word address, Word value) {
    //std::cout << "Mem Write: Addr=" << address << " Value=" << value << std::endl;
    if (address < MEM_SIZE) {
        data[address] = value;

        // Video mapiranje: Ako softver piše u područje [8192, 10192)
        // To je tekstualni/grafički ispis. Za početak, ispisujemo u konzolu.
        if (address >= 8192 && address < 8192 + 2000) {
            // Ispisuj samo ako je validan karakter
            if (value >= 32 && value < 127) {
                std::cout << (char)value << std::flush;
            }
            // Podrška za novi red
            else if (value == 10 || value == 13) {
                std::cout << std::endl;
            }
        }
    }
}

// Dodajemo metodu za učitavanje ROM/RAM fajla
bool Memory::loadFromFile(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) return false;
    
    // Učitavamo direktno u naš vektor (maksimalno MEM_SIZE riječi)
    fread(data.data(), sizeof(Word), MEM_SIZE, file);
    fclose(file);
    return true;
}

Word* Memory::getRawPointer(Word address) {
    return &data[address];
}