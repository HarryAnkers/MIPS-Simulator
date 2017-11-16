#include "j_instruction.hpp"

using namespace std;

j_instruction::j_instruction(string instruction){
    opcode=0;
    immediate=0;
    
    for(int i=0; i<32; i++){
        if(i<6){
            if(instruction[i]=='1'){
                opcode+=pow(2,5-i);
            }
        } else {
            if(instruction[i]=='1'){
                immediate+=pow(2,31-i);
            }
        }
    }
}

j_instruction::~j_instruction(){}

void j_instruction::pc_inc(uint32_t &pc){
    pc+=4;
}

int j_instruction::run(uint32_t *regs, uint32_t &pc){
    int returnval = 0;
    
    pc_inc(pc);
    return returnval;
}


