#include "i_instruction.hpp"
#define dataoffset 0x4000000
#define datasize 0x4000000

i_instruction::i_instruction(string instruction){
    opcode=0;
    source1=0;
    dest=0;
    uimmediate=0;
    
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
                dest+=pow(2,15-i);
            }
        } else {
            if(instruction[i]=='1'){
                uimmediate+=pow(2,31-i);
            }
        }
    }
    
    simmediate=(~uimmediate)+1;
}

i_instruction::~i_instruction(){}

void i_instruction::pc_inc(uint32_t &pc){
    pc+=4;
}

int i_instruction::store(uint32_t *data, uint32_t dataval ,int datalength,uint32_t addr){
    int startloc = -1;
    uint32_t temp = 0;
    if(addr>=dataoffset){
        addr=-dataoffset;
        if(datalength==32){
            if(addr+4<=datasize){
                dataval=data[addr/4];
            }else{ return -11; }
        } else if(datalength==16){
            if(addr+1<=datasize){
                startloc=addr%4;
                switch(startloc){
                    case 0:
                        temp=data[addr/4];
                        temp=temp&(0xFFFF0000);
                        temp+=temp+dataval;
                        data[addr/4]=temp;
                        break;
                    case 1:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFF0000FF);
                        temp+=temp+(dataval<<8);
                        data[(addr-startloc)/4]=temp;
                        break;
                    case 2:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x0000FFFF);
                        temp+=temp+(dataval<<16);
                        data[(addr-startloc)/4]=temp;
                        break;
                    case 3:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFFFF);
                        temp+=temp+(dataval<<24);
                        data[(addr-startloc)/4]=temp;
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0xFFFFFF00);
                        temp+=temp+(dataval>>8);
                        data[((addr-startloc)/4)+1]=temp;
                        break;
                }
            }else{ return -11; }
        } else if(datalength==8){
            if(addr<=datasize){
                startloc=addr%4;
                switch(startloc){
                    case 0:
                        temp=data[addr/4];
                        temp=temp&(0xFFFFFF00);
                        temp+=temp+dataval;
                        data[addr/4]=temp;
                        break;
                    case 1:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFF00FF);
                        temp+=temp+(dataval<<8);
                        data[(addr-startloc)/4]=temp;
                        break;
                    case 2:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFF00FFFF);
                        temp+=temp+(dataval<<16);
                        data[(addr-startloc)/4]=temp;
                        break;
                    case 3:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x00FFFFFF);
                        temp+=temp+(dataval<<24);
                        data[(addr-startloc)/4]=temp;
                    break;
                }
            }else{ return -11; }
        }else{ return -12; }
    }else{ return -12; }
    return 0;
}

void i_instruction::BLTZ(uint32_t *regs, uint32_t &pc){
    int32_t temp1 = (~regs[source1])+1;
    if(temp1<0){
        int32_t temp2 = simmediate<<2;
        pc+=temp2;
    }
}

void i_instruction::BGEZ(uint32_t *regs, uint32_t &pc){
    int32_t temp1 = (~regs[source1])+1;
    if(temp1>=0){
        int32_t temp2 = simmediate<<2;
        pc+=temp2;
    }
}

void i_instruction::BLTZAL(uint32_t *regs, uint32_t &pc){
    int32_t temp1 = (~regs[source1])+1;
    if(temp1<0){
        int32_t temp2 = simmediate<<2;
        regs[31]=pc+8;
        pc+=temp2;
    }
}

void i_instruction::BGEZAL(uint32_t *regs, uint32_t &pc){
    int32_t temp1 = (~regs[source1])+1;
    if(temp1>=0){
        int32_t temp2 = simmediate<<2;
        regs[31]=pc+8;
        pc+=temp2;
    }
}


void i_instruction::BEQ(uint32_t *regs, uint32_t &pc){
    if(regs[source1]==regs[dest]){
        int32_t temp = simmediate<<2;
        pc+=temp;
    }
}

void i_instruction::BNE(uint32_t *regs, uint32_t &pc){
    if(regs[source1]!=regs[dest]){
        int32_t temp = simmediate<<2;
        pc+=temp;
    }
}

void i_instruction::BLEZ(uint32_t *regs, uint32_t &pc){
    int32_t temp1 = (~regs[source1])+1;
    if(temp1<=0){
        int32_t temp2 = simmediate<<2;
        pc+=temp2;
    }
}

void i_instruction::BGTZ(uint32_t *regs, uint32_t &pc){
    int32_t temp1 = (~regs[source1])+1;
    if(temp1>0){
        int32_t temp2 = simmediate<<2;
        pc+=temp2;
    }
}

int i_instruction::ADDI(uint32_t *regs){
    int returnval=0;
    
    regs[dest]=regs[source1]+simmediate;
    
    if((regs[source1]&(0x80000000))==
       (simmediate&(0x8000))){
        
        if((regs[source1]&(0x80000000))!=
           (regs[dest]&(0x80000000))){
            
            returnval=-10;
        }
    }
    return returnval;
}

void i_instruction::ADDIU(uint32_t *regs){
    regs[dest]=regs[source1]+simmediate;
}

void i_instruction::SLTI(uint32_t *regs){
    int32_t temp=(~regs[source1])+1;
    if(temp<simmediate){
        regs[dest]=1;
    }else {
        regs[dest]=0;
    }
}

void i_instruction::SLTIU(uint32_t *regs){
    if(regs[source1]<uimmediate){
        regs[dest]=1;
    }else {
        regs[dest]=0;
    }
}

void i_instruction::ANDI(uint32_t *regs){
    uint32_t temp = uimmediate<<16;
    regs[dest]=regs[source1]&temp;
}

void i_instruction::ORI(uint32_t *regs){
    uint32_t temp = uimmediate<<16;
    regs[dest]=regs[source1]|temp;
}

void i_instruction::XORI(uint32_t *regs){
    uint32_t temp = uimmediate<<16;
    regs[dest]=regs[source1]|temp;
}

int i_instruction::x(uint32_t *regs, uint32_t *data){
    
    return 0;
}

int i_instruction::run(uint32_t *regs, uint32_t &pc){
    int returnval = 0;
    
    uint32_t data[100] = {3};
    
    switch(opcode){
        case 0x1:
            if(regs[dest]==0){
                BLTZ(regs, pc);
                pc_inc(pc);
            } else if(regs[dest]==1){
                BGEZ(regs, pc);
                pc_inc(pc);
            } else if(regs[dest]==16){
                BLTZAL(regs, pc);
                pc_inc(pc);
            } else if(regs[dest]==17){
                BGEZAL(regs, pc);
                pc_inc(pc);
            } else {
                returnval = -12;
            }
            break;
        case 0x4:
            BEQ(regs, pc);
            pc_inc(pc);
            break;
        case 0x5:
            BNE(regs, pc);
            pc_inc(pc);
            break;
        case 0x6:
            if(regs[dest]==0){
                BLEZ(regs, pc);
                pc_inc(pc);
            } else {
                returnval = -12;
            }
            break;
        case 0x7:
            if(regs[dest]==0){
                BGTZ(regs, pc);
                pc_inc(pc);
            } else {
                returnval = -12;
            }
            break;
        case 0x8:
            returnval = ADDI(regs);
            break;
        case 0x9:
            ADDIU(regs);
            break;
        case 0x0A:
            SLTI(regs);
            pc_inc(pc);
            break;
        case 0x0B:
            SLTIU(regs);
            pc_inc(pc);
            break;
        case 0x0C:
            ANDI(regs);
            pc_inc(pc);
            break;
        case 0x0D:
            ORI(regs);
            pc_inc(pc);
            break;
        case 0x0E:
            XORI(regs);
            pc_inc(pc);
            break;
        case 0x2B:
            SW(regs, data);
            pc_inc(pc);
            break;
    }
    pc_inc(pc);
    return returnval;
}
