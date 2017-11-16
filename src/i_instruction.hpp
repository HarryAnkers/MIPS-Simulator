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
    int run(uint32_t *regs, uint32_t &pc );
    
    int ADDI(uint32_t *regs);
    void ADDIU(uint32_t *regs);
    void SLTI(uint32_t *regs);
    void SLTIU(uint32_t *regs);
    void ANDI(uint32_t *regs);
    void ORI(uint32_t *regs);
    void XORI(uint32_t *regs);
    
//private:
    int opcode;
    int source1;
    int dest;
    uint16_t uimmediate;
    int16_t simmediate;
};


#endif
