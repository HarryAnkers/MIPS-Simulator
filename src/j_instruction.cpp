#include "j_instruction.hpp"

using namespace std;

j_instruction::j_instruction(uint32_t instruction){
    //decodes the instructions into components
    opcode=instruction>>26;
    
    immediate=instruction<<6;
    immediate=immediate>>6;
}

j_instruction::~j_instruction(){}

void j_instruction::J(uint32_t &pc){
    //shifts pc to keep only two msb
    pc=pc>>28;
    pc=pc<<28;
    
    //adds the unsigned immediate to pc
    pc=pc+immediate;
}

void j_instruction::JAL(uint32_t *regs, uint32_t &pc){
    //saves pc
    regs[31]=pc+8;
    
    //shifts pc to keep only two msb    pc=pc>>28;
    pc=pc<<28;
    
    //adds the unsigned immediate to pc
    pc=pc+immediate;
}

int j_instruction::run(uint32_t *regs, uint32_t &pc){
    int returnval = 0;
    
    //chooses function
    if(opcode==2){
        J(pc);
    } else if(opcode==3){
        JAL(regs,pc);
    } else{
        //if no func is selected returns an error (should never reach)
        returnval = -12;
    }
    
    return returnval;
}


