#ifndef instruction_hpp
#define instruction_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

class r_instruction{
public:
    r_instruction(string instruction);
    ~r_instruction();
    
    void pc_inc(uint32_t &pc);
    int run(uint32_t *regs, uint32_t &pc );
    
    int SLL(uint32_t *regs);
    int SRL(uint32_t *regs);
    int SRA(uint32_t *regs);
    int SLLV(uint32_t *regs);
    int SRLV(uint32_t *regs);
    int SRAV(uint32_t *regs);
    int JR(uint32_t* regs, uint32_t &pc);
    int ADD(uint32_t *regs);
    int ADDU(uint32_t *regs);
    int SUB(uint32_t *regs);
    int SUBU(uint32_t *regs);
    int AND(uint32_t *regs);
    int OR(uint32_t *regs);
    int XOR(uint32_t *regs);
    int SLT(uint32_t *regs);
    int SLTU(uint32_t *regs);
    
    
//private:
    int opcode;
    int source1;
    int source2;
    int dest;
    int shift;
    int func;
};


#endif



