#pragma once
#include "common.h"
#include "disk.h"
#include "video.h"
#include <vector>
#include <string>

class Memory {
private:
    std::vector<Word> data;
    DiskDevice disk;    // Disk uredjaj
    VideoDevice video;  // Video uredjaj (80x25)

public:
    Memory();
    Word read(Word address);
    void write(Word address, Word value);
    
    bool loadFromFile(const std::string& filename);
    
    Word* getRawPointer(Word address);
};