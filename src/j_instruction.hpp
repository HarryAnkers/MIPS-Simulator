#ifndef j_instruction_hpp
#define j_instruction_hpp

#include <stdio.h>
#include <string>
#include <cmath>

using namespace std;

class j_instruction{
public:
    j_instruction(string instruction);
    ~j_instruction();
    
    int run(uint32_t *regs, uint32_t &pc);
    
    void J(uint32_t &pc);
    void JAL(uint32_t *regs, uint32_t &pc);
    
//private:
    int opcode;
    int immediate;
};

#endif
