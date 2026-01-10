#pragma once
#include "common.h"
#include <vector>
#include <string>

class Memory {
private:
    std::vector<Word> data;

public:
    Memory();
    Word read(Word address);
    void write(Word address, Word value);
    
    bool loadFromFile(const std::string& filename);
    
    Word* getRawPointer(Word address);
};