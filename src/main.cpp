#include <iostream>
#include "memory.h"
#include "cpu.h"

int main(int argc, char* argv[]) {
    std::cout << "--- System Virtual Machine ---" << std::endl;

    Memory ram;

    // 1. Učitavanje binarnog fajla (forth.mem ili neki drugi ROM)
    std::string bootFile = "forth.mem";
    if (!ram.loadFromFile(bootFile)) {
        std::cerr << "GRESKA: Ne mogu ucitati " << bootFile << std::endl;
        std::cerr << "Provjerite da li je fajl u istom folderu kao i .exe" << std::endl;
        return 1;
    }
    std::cout << "Uspjesno ucitan: " << bootFile << std::endl;

    // 2. Inicijalizacija CPU-a sa memorijom
    CPU cpu(ram);

    // TEST: Ručno upisujemo instrukcije koje kažu:
    ram.write(8192, 72); // 'H'
    ram.write(8193, 69); // 'E'
    ram.write(8194, 76); // 'L'
    ram.write(8195, 76); // 'L'
    ram.write(8196, 79); // 'O'

    // 3. Pokretanje CPU-a
    std::cout << "CPU Start..." << std::endl;
    try {
        cpu.run();
    } catch (const std::exception& e) {
        std::cerr << "Kriticna greska u radu CPU-a: " << e.what() << std::endl;
    }

    std::cout << "\nSystem Virtual Machine - Kraj" << std::endl;
    return 0;
}