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
    
    void pc_inc(uint32_t &pc);
    int run(uint32_t *regs, uint32_t &pc );
    
//private:
    int opcode;
    int immediate;
};

#endif
