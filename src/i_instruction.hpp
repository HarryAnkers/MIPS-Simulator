#ifndef i_instruction_hpp
#define i_instruction_hpp

#include <stdio.h>
#include <cmath>
#include <string>

#define dataoffset 0x4000000
#define datasize 0x4000000
#define instsize 0x1000000

using namespace std;

class i_instruction{
public:
    i_instruction(uint32_t i_instruction);
    
    ~i_instruction();
    
    //function to inc pc and select op
    void pc_inc(uint32_t &pc);
    int run(uint32_t *data, uint32_t *regs, uint32_t &pc, uint32_t &getc, uint32_t &putc);
    
    //seperate ops for storing and loading
    int store(uint32_t *data, uint32_t dataval ,int datalength,uint32_t addr);
    int load(uint32_t *data, uint32_t &returndata ,int datalength,uint32_t addr);
    
    //all possible ops
    void BLTZ(uint32_t *regs, uint32_t &pc);
    void BGEZ(uint32_t *regs, uint32_t &pc);
    void BLTZAL(uint32_t *regs, uint32_t &pc);
    void BGEZAL(uint32_t *regs, uint32_t &pc);
    void BEQ(uint32_t *regs, uint32_t &pc);
    void BNE(uint32_t *regs, uint32_t &pc);
    void BLEZ(uint32_t *regs, uint32_t &pc);
    void BGTZ(uint32_t *regs, uint32_t &pc);
    void  ADDI(uint32_t *regs);
    void ADDIU(uint32_t *regs);
    void SLTI(uint32_t *regs);
    void SLTIU(uint32_t *regs);
    void ANDI(uint32_t *regs);
    void ORI(uint32_t *regs);
    void XORI(uint32_t *regs);
    void LUI(uint32_t *regs);
    int LB(uint32_t *regs, uint32_t *data);
    int LH(uint32_t *regs, uint32_t *data);
    int LWL(uint32_t *regs, uint32_t *data);
    int LW(uint32_t *regs, uint32_t *data);
    int LBU(uint32_t *regs, uint32_t *data);
    int LHU(uint32_t *regs, uint32_t *data);
    int LWR(uint32_t *regs, uint32_t *data);
    int SB(uint32_t *regs, uint32_t *data);
    int SH(uint32_t *regs, uint32_t *data);
    int SW(uint32_t *regs, uint32_t *data);
    
//variables
    uint32_t opcode;
    uint32_t source1;
    uint32_t dest;
    uint16_t uimmediate;
    int16_t simmediate;
};


#endif
