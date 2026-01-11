#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"
#include "memory.h"

class Keyboard {
public:
    explicit Keyboard(Memory& memory);
    void processInput();

private:
    Memory& memory;
    Word cursor;
};

#endif