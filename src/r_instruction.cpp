#include "r_instruction.hpp"

using namespace std;

r_instruction::r_instruction(string instruction){
    opcode=0;
    source1=0;
    source2=0;
    dest=0;
    shift=0;
    func=0;
    
    for(int i=0; i<32; i++){
        if(i<6){
            if(instruction[i]=='1'){
                opcode+=pow(2,5-i);
            }
        } else if(i<11){
            if(instruction[i]=='1'){
                source1+=pow(2,10-i);
            }
        } else if(i<16){
            if(instruction[i]=='1'){
                source2+=pow(2,15-i);
            }
        } else if(i<21){
            if(instruction[i]=='1'){
                dest+=pow(2,20-i);
            }
        } else if(i<26){
            if(instruction[i]=='1'){
                shift+=pow(2,25-i);
            }
        } else{
            if(instruction[i]=='1'){
                func+=pow(2,31-i);
            }
        }
    }
}

r_instruction::~r_instruction(){}

void r_instruction::pc_inc(uint32_t &pc){
    pc+=4;
}

int r_instruction::SLL(uint32_t* regs){
    if(source1!=0){
        return -12;
    } else {
        regs[dest]=regs[source2]<<shift;
        return 0;
    }
}

int r_instruction::SRL(uint32_t* regs){
    if(source1!=0){
        return -12;
    } else {
        regs[dest]=regs[source2]>>shift;
        return 0;
    }
}

int r_instruction::SRA(uint32_t* regs){
    if(source1!=0){
        return -12;
    } else {
        for(int i=0; i<shift; i++){
            regs[dest]=regs[source2]>>1;
            if((source2&0x80000000)!=0){
                regs[dest]+=0x80000000;
            }
        }
        return 0;
    }
}

int r_instruction::SLLV(uint32_t* regs){
    if(dest!=0){
        return -12;
    } else {
        regs[dest]=regs[source2]<<regs[source1];
        return 0;
    }
}

int r_instruction::SRLV(uint32_t* regs){
    if(dest!=0){
        return -12;
    } else {
        regs[dest]=regs[source2]>>regs[source1];
        return 0;
    }
}

int r_instruction::SRAV(uint32_t* regs){
    if(shift!=0){
        return -12;
    } else {
        for(int i=0; i<regs[source1]; i++){
            regs[dest]=regs[source2]>>1;
            if((source2&0x80000000)!=0){
                regs[dest]+=0x80000000;
            }
        }
        return 0;
    }
}

int r_instruction::JR(uint32_t* regs, uint32_t &pc){
    if(((source1!=0)&&((source1!=0)))&&(shift!=0)){
        return -12;
    } else {
        pc=regs[source1];
        return 0;
    }
}

int r_instruction::JALR(uint32_t* regs, uint32_t &pc){
    if(((source1!=0)&&((source1!=0)))&&(shift!=0)){
        return -12;
    } else {
        regs[31]=pc+8;
        pc=regs[source1];
        return 0;
    }
}

int r_instruction::ADD(uint32_t *regs){
    if(shift!=0){
        return -12;
    } else{
        int returnval=0;
    
        regs[dest]=regs[source1]+regs[source2];
    
        if((regs[source1]&(0x80000000))==
            (regs[source2]&(0x80000000))){
           
           if((regs[source1]&(0x80000000))!=
              (regs[dest]&(0x80000000))){
               
               returnval=-10;
           }
        }
        return returnval;
    }
}

int r_instruction::ADDU(uint32_t *regs){
     if(shift!=0){
         return -12;
     } else {
         regs[dest]=regs[source1]+regs[source2];
         return 0;
     }
}

int r_instruction::SUB(uint32_t *regs){
    if(shift!=0){
        return -12;
    }
    else{
        int returnval=0;
    
        regs[dest]=regs[source1]-regs[source2];
    
        if((regs[source1]&(0x80000000))!=
           (regs[source2]&(0x80000000))){
            
            if((regs[source1]&(0x80000000))!=
               (regs[dest]&(0x80000000))){
                
                returnval=-10;
            }
        }
        return returnval;
    }
}

int r_instruction::SUBU(uint32_t *regs){
    if(shift!=0){
        return -12;
    }else{
        regs[dest]=regs[source1]-regs[source2];
        return 0;
    }
}

int r_instruction::AND(uint32_t *regs){
    if(shift!=0){
        return -12;
    } else {
        regs[dest]=regs[source1]&regs[source2];
        
        return 0;
    }
}

int r_instruction::OR(uint32_t *regs){
    if(shift!=0){
        return -12;
    } else {
        regs[dest]=regs[source1]|regs[source2];
        return 0;
    }
}

int r_instruction::XOR(uint32_t *regs){
    if(shift!=0){
        return -12;
    } else {
        regs[dest]=regs[source1]^regs[source2];
        return 0;
    }
}

int r_instruction::SLT(uint32_t *regs){
    bool r1sign, r2sign;
    
    if(shift!=0){
        return -12;
    } else {
        r1sign=(regs[source1]&(0x80000000))==0;
        r2sign=(regs[source2]&(0x80000000))==0;
        
        if(r1sign!=r2sign){
            if(r1sign==true){
                regs[dest]=0;
            } else {
                regs[dest]=1;
            }
        } else if(r1sign==true){
            if (regs[source1]<regs[source2]) {
                regs[dest]=1;
            } else {
                regs[dest]=0;
            }
        } else {
            if (regs[source1]>regs[source2]) {
                regs[dest]=1;
            } else {
                regs[dest]=0;
            }
        }
        return 0;
    }
}

int r_instruction::SLTU(uint32_t *regs){
    if(shift!=0){
        return -12;
    } else {
        if(regs[source1]<regs[source2]){
            regs[dest]=1;
        } else {
            regs[dest]=0;
        }
        return 0;
    }
}

int r_instruction::run(uint32_t *regs, uint32_t &pc){
    int returnval = 0;
    switch(func){
        case 0x0:
            returnval = SLL(regs);
            pc_inc(pc);
            break;
        case 0x2:
            returnval = SRL(regs);
            pc_inc(pc);
            break;
        case 0x3:
            returnval = SRA(regs);
            pc_inc(pc);
            break;
        case 0x4:
            returnval = SLLV(regs);
            pc_inc(pc);
            break;
        case 0x6:
            returnval = SRLV(regs);
            pc_inc(pc);
            break;
        case 0x7:
            returnval = SRAV(regs);
            pc_inc(pc);
            break;
        case 0x08:
            returnval = JR(regs, pc);
            break;
        case 0x09:
            returnval = JALR(regs, pc);
            break;
        case 0x20:
            returnval = ADD(regs);
            pc_inc(pc);
            break;
        case 0x21:
            returnval = ADDU(regs);
            pc_inc(pc);
            break;
        case 0x22:
            returnval = SUB(regs);
            pc_inc(pc);
            break;
        case 0x23:
            returnval = SUBU(regs);
            pc_inc(pc);
            break;
        case 0x24:
            returnval = AND(regs);
            pc_inc(pc);
            break;
        case 0x25:
            returnval = OR(regs);
            pc_inc(pc);
            break;
        case 0x26:
            returnval = XOR(regs);
            pc_inc(pc);
            break;
        case 0x2A:
            returnval = SLT(regs);
            pc_inc(pc);
            break;
        case 0x2B:
            returnval = SLTU(regs);
            pc_inc(pc);
            break;
    }
    return returnval;
}
