#ifndef instruction_hpp
#define instruction_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

class r_instruction{
public:
    r_instruction(uint32_t instruction);
    ~r_instruction();
    
    //functions to choose function and inc pc counter
    void pc_inc(uint32_t &pc);
    int run(uint32_t &HI, uint32_t &LO, uint32_t *regs, uint32_t &pc );
    
    //all possible ops
    int SLL(uint32_t *regs);
    int SRL(uint32_t *regs);
    int SRA(uint32_t *regs);
    int SLLV(uint32_t *regs);
    int SRLV(uint32_t *regs);
    int SRAV(uint32_t *regs);
    int JR(uint32_t *regs, uint32_t &pc);
    int JALR(uint32_t *regs, uint32_t &pc);
    int MFHI(uint32_t *regs, uint32_t &HI);
    int MTHI(uint32_t *regs, uint32_t &HI);
    int MFLO(uint32_t *regs, uint32_t &LO);
    int MTLO(uint32_t *regs, uint32_t &LO);
    int MULT(uint32_t *regs, uint32_t &HI, uint32_t &LO);
    int MULTU(uint32_t *regs, uint32_t &HI, uint32_t &LO);
    int DIV(uint32_t *regs, uint32_t &HI, uint32_t &LO);
    int DIVU(uint32_t *regs, uint32_t &HI, uint32_t &LO);
    int ADD(uint32_t *regs);
    int ADDU(uint32_t *regs);
    int SUB(uint32_t *regs);
    int SUBU(uint32_t *regs);
    int AND(uint32_t *regs);
    int OR(uint32_t *regs);
    int XOR(uint32_t *regs);
    int SLT(uint32_t *regs);
    int SLTU(uint32_t *regs);
    
    //variables
    int opcode;
    int source1;
    int source2;
    int dest;
    int shift;
    int func;
};


#endif
