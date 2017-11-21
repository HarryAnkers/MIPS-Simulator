#ifndef i_instruction_hpp
#define i_instruction_hpp

#include <stdio.h>
#include <cmath>
#include <string>

using namespace std;

class i_instruction{
public:
    i_instruction(string i_instruction);
    
    ~i_instruction();
    
    void pc_inc(uint32_t &pc);
    int run(uint32_t *regs, uint32_t &pc);
    
    int store(uint32_t *data, uint32_t dataval ,int datalength,uint32_t addr);
    
    void BLTZ(uint32_t *regs, uint32_t &pc);
    void BGEZ(uint32_t *regs, uint32_t &pc);
    void BLTZAL(uint32_t *regs, uint32_t &pc);
    void BGEZAL(uint32_t *regs, uint32_t &pc);
    void BEQ(uint32_t *regs, uint32_t &pc);
    void BNE(uint32_t *regs, uint32_t &pc);
    void BLEZ(uint32_t *regs, uint32_t &pc);
    void BGTZ(uint32_t *regs, uint32_t &pc);
    int ADDI(uint32_t *regs);
    void ADDIU(uint32_t *regs);
    void SLTI(uint32_t *regs);
    void SLTIU(uint32_t *regs);
    void ANDI(uint32_t *regs);
    void ORI(uint32_t *regs);
    void XORI(uint32_t *regs);
    int SW(uint32_t *regs, uint32_t *data);
    
//private:
    int opcode;
    int source1;
    int dest;
    uint16_t uimmediate;
    int16_t simmediate;
};


#endif
