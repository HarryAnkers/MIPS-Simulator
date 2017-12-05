#ifndef j_instruction_hpp
#define j_instruction_hpp

#include <stdio.h>
#include <string>
#include <cmath>

using namespace std;

class j_instruction{
public:
    j_instruction(uint32_t instruction);
    ~j_instruction();
    
    //func to choose op
    void run(uint32_t *regs, uint32_t &pc);
    
    //only possible functions
    void J(uint32_t &pc);
    void JAL(uint32_t *regs, uint32_t &pc);
    
//private:
    uint32_t opcode;
    uint32_t immediate;
};

#endif
