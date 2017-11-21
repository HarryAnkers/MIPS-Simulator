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

void j_instruction::J(uint32_t &pc){
    pc=pc>>28;
    pc=pc<<28;
    uint32_t temp = immediate;
    pc=pc+immediate;
}

void j_instruction::JAL(uint32_t *regs, uint32_t &pc){
    regs[31]=pc+8;
    pc=pc>>28;
    pc=pc<<28;
    uint32_t temp = immediate;
    pc=pc+temp;
}

int j_instruction::run(uint32_t *regs, uint32_t &pc){
    int returnval = 0;
    
    if(opcode==2){
        J(pc);
    } else if(opcode==3){
        JAL(regs,pc);
    } else{
        returnval = -12;
    }
    
    return returnval;
}


