#include "i_instruction.hpp"

i_instruction::i_instruction(uint32_t instruction){
    //decodes the instructions into components
    opcode=instruction>>26;

    source1=instruction<<6;
    source1=source1>>27;

    dest=instruction<<11;
    dest=dest>>27;

    uimmediate=instruction<<16;
    uimmediate=uimmediate>>16;
    
    simmediate=uimmediate;
}

i_instruction::~i_instruction(){}

void i_instruction::pc_inc(uint32_t &pc){
    pc+=4;
}

//store function for all data storage
int i_instruction::store(uint32_t *data, uint32_t dataval ,int datalength,uint32_t addr){
    int startloc = -1;
    uint32_t temp = 0;
    
    //if write addr is ADDR_NULL it returns -1
    if(addr==0x30000000){
        return -1;
    }
    
    //checks the address is after the data offset
    if(addr>=dataoffset){
        //removes offset
        addr=-dataoffset;
        
        //all the following works via anding the segment of the word that will be stored with 0
        //it then shifts the word, half word or byte accordingly and adds it
        
        //startloc is the byte of the word it starts from
        startloc=addr%4;
        
        //word case
        if(datalength==32){
            //checks if the word (and if needed the word proceeding) is within the size
            if(((addr%4==0)&&(addr+3<datasize))||(addr+(7-addr%4)<datasize)){
                switch(startloc){
                    //ls byte and ms 3 bytes of the following word
                    case 3:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFFFF00);
                        temp+=(dataval>>24);
                        data[(addr-startloc)/4]=temp;
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0x000000FF);
                        temp+=(dataval<<8);
                        data[((addr-startloc)/4)+1]=temp;
                        break;
                    //ls 2 bytes and ms 2 bytes of the following word
                    case 2:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFF0000);
                        temp+=(dataval>>16);
                        data[addr/4]=temp;
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0x0000FFFF);
                        temp+=(dataval<<16);
                        data[((addr-startloc)/4)+1]=temp;
                        break;
                    //ls 3 bytes and ms byte of the following word
                    case 1:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFF000000);
                        temp+=(dataval>>8);
                        data[(addr-startloc)/4]=temp;
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0x00FFFFFF);
                        temp+=(dataval<<24);
                        data[((addr-startloc)/4)+1]=temp;
                        break;
                    //the full word
                    case 0:
                        data[(addr-startloc)/4]=dataval;
                        break;
                }
            }else{
                //word outside datasize
                exit(-11);
            }
        //half word case
        } else if(datalength==16){
            //checks if the word (and if needed the word proceeding) is within the size
            if(((addr%4==3)&&(addr+(7-addr%4)<datasize))||(addr+(3-addr%4)<datasize)){
                switch(startloc){
                    //ls byte and ms byte of the following word
                    case 3:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFFFF00);
                        temp+=(dataval>>8);
                        data[(addr-startloc)/4]=temp;
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0x00FFFFFF);
                        temp+=(dataval<<24);
                        data[((addr-startloc)/4)+1]=temp;
                        break;
                    //ls 2 bytes
                    case 2:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFF0000);
                        temp+=dataval;
                        data[addr/4]=temp;
                        break;
                    //2nd and 3rd bytes
                    case 1:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFF0000FF);
                        temp+=(dataval<<8);
                        data[(addr-startloc)/4]=temp;
                        break;
                    //ms 2 bytes
                    case 0:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x0000FFFF);
                        temp+=(dataval<<16);
                        data[(addr-startloc)/4]=temp;
                        break;
                }
            }else{
                //word outside datasize
                exit(-11);
            }
        //byte case
        } else if(datalength==8){
            //checks within data size
            if((addr-(addr%4))<=datasize){
                switch(startloc){
                    //ls byte
                    case 3:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFFFF00);
                        temp+=dataval;
                        data[addr/4]=temp;
                        break;
                    //2nd ls byte
                    case 2:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFF00FF);
                        temp+=(dataval<<8);
                        data[(addr-startloc)/4]=temp;
                        break;
                    //2nd ms byte
                    case 1:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFF00FFFF);
                        temp+=(dataval<<16);
                        data[(addr-startloc)/4]=temp;
                        break;
                    //ms byte
                    case 0:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x00FFFFFF);
                        temp+=(dataval<<24);
                        data[(addr-startloc)/4]=temp;
                    break;
                }
            }else{
                //word outside datasize
                exit(-11);
            }
        //only need word, half-word and byte if none throws error
        }else{
            //has picked a different type of function (should never hit)
            exit(-20);
        }
    //error for less than range
    }else{
        //word outside datasize
        exit(-11);
    }
    return 0;
}

//load function for all data retrival
int i_instruction::load(uint32_t *data, uint32_t &returndata,int datalength,uint32_t addr){
    uint32_t temp = 0;
    int startloc = 0;
    
    //if read addr is ADDR_GETC it returns 3
    if(addr==0x30000000){
        return 3;
    }
    
    //checks data is not less than the offset
    if(addr>=dataoffset){
        //removes the offset
        addr=-dataoffset;
        
        //works via anding the desired segment of the loaded word with 1 and all else with 0
        //then shifts it as required and retrives the rest of the (w/hw/b) from the
        //next word if required and data is returned in the form of changing a variable called by ref
        
        //startloc is the byte it begins to read from
        startloc=addr%4;
        
        //word case
        if(datalength==32){
            //checks it is lower than the data size boundary
            if(((addr%4==0)&&(addr+3<datasize))||(addr+(7-addr%4)<datasize)){
                switch(startloc){
                    //loads the ls byte and the 3 ms bytes from the following word
                    case 3:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x000000FF);
                        temp=(temp<<24);
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0xFFFFFF00);
                        temp+=(temp>>24);
                        returndata=temp;
                        break;
                    //loads the 2 ls bytes and the 2 ms bytes from the following word
                    case 2:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x0000FFFF);
                        temp=(temp<<16);
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0xFFFF0000);
                        temp+=(temp>>16);
                        returndata=temp;
                        break;
                    //loads the 3 ls bytes and the ms byte from the following word
                    case 1:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x00FFFFFF);
                        temp=(temp<<8);
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0xFF000000);
                        temp+=(temp>>24);
                        returndata=temp;
                        break;
                    //loads the full word
                    case 0:
                        returndata = data[(addr-startloc)/4];
                        break;
                }
            } else {
                //word outside datasize
                exit(-11);
            }
        //half word case
        } else if(datalength==16){
            //checks it is lower than the data size boundary
            if(((addr%4==3)&&((addr+(7-addr%4))<datasize))||(addr+(3-addr%4)<datasize)){
                switch(startloc){
                    //loads the ls byte and the ms byte from the following word
                    case 3:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x000000FF);
                        temp=(temp<<8);
                        
                        temp=data[((addr-startloc)/4)+1];
                        temp=temp&(0xFF000000);
                        temp+=(temp>>24);
                        returndata=temp;
                        break;
                    //loads the 2 ls bytes
                    case 2:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x0000FFFF);
                        returndata=temp;
                        break;
                    //loads the 2nd and 3rd bytes
                    case 1:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x00FFFF00);
                        temp=(temp>>8);
                        returndata=temp;
                        break;
                    //loads the 2 ms bytes
                    case 0:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFFFF0000);
                        temp=(temp>>16);
                        returndata=temp;
                        break;
                }
            } else {
                //word outside datasize
                exit(-11);
            }
        //byte case
        } else if(datalength==8){
            if((addr-(addr%4))<=datasize){
                switch(startloc){
                    //loads the ls byte
                    case 3:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x000000FF);
                        returndata = temp;
                        break;
                    //loads the 2nd ls byte
                    case 2:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x0000FF00);
                        temp=(temp>>8);
                        returndata = temp;
                        break;
                    //loads the 2nd ms byte
                    case 1:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0x00FF0000);
                        temp=(temp>>16);
                        returndata = temp;
                        break;
                    //loads the ms byte
                    case 0:
                        temp=data[(addr-startloc)/4];
                        temp=temp&(0xFF000000);
                        temp=(temp>>24);
                        returndata = temp;
                        break;
                }
            } else {
                //word outside datasize
                exit(-11);
            }
        //only need word, half-word and byte case throw internal error if not
        } else {
            //has picked a different type of function (should never hit)
            exit(-20);
        }
    //error for if less than offset
    } else {
        //word outside datasize
        exit(-11);
    }
    return 0;
}

void i_instruction::BLTZ(uint32_t *regs, uint32_t &pc){
    //convert reg value to signed
    int32_t temp1 = regs[source1];
    
    //if less than it jumps the pc
    if(temp1<0){
        int32_t temp2 = simmediate<<2;
        pc+=temp2;
    }
}

void i_instruction::BGEZ(uint32_t *regs, uint32_t &pc){
    //convert reg value to signed
    int32_t temp1 = regs[source1];
    
    //if more than or equal it jumps the pc
    if(temp1>=0){
        int32_t temp2 = simmediate<<2;
        pc+=temp2;
    }
}

void i_instruction::BLTZAL(uint32_t *regs, uint32_t &pc){
    //convert reg value to signed
    int32_t temp1 = regs[source1];
    
    //if less than it jumps the pc and saves the pc value
    if(temp1<0){
        int32_t temp2 = simmediate<<2;
        regs[31]=pc+8;
        pc+=temp2;
    }
}

void i_instruction::BGEZAL(uint32_t *regs, uint32_t &pc){
    //convert reg value to signed
    int32_t temp1 = regs[source1];
    
    //if more than or equal it jumps the pc and saves the pc value
    if(temp1>=0){
        int32_t temp2 = simmediate<<2;
        regs[31]=pc+8;
        pc+=temp2;
    }
}

void i_instruction::BEQ(uint32_t *regs, uint32_t &pc){
    //if equal it jumps the pc
    if(regs[source1]==regs[dest]){
        int32_t temp = simmediate<<2;
        pc+=temp;
    }
}

void i_instruction::BNE(uint32_t *regs, uint32_t &pc){
    //if not equal it jumps the pc
    if(regs[source1]!=regs[dest]){
        int32_t temp = simmediate<<2;
        pc+=temp;
    }
}

void i_instruction::BLEZ(uint32_t *regs, uint32_t &pc){
    //convert reg value to signed
    int32_t temp1 = regs[source1];
    
    //if less than or equal it jumps the pc
    if(temp1<=0){
        int32_t temp2 = simmediate<<2;
        pc+=temp2;
    }
}

void i_instruction::BGTZ(uint32_t *regs, uint32_t &pc){
    //convert reg value to signed
    int32_t temp1 = regs[source1];
    
    //if more than it jumps the pc
    if(temp1>0){
        int32_t temp2 = simmediate<<2;
        pc+=temp2;
    }
}

void i_instruction::ADDI(uint32_t *regs){
    //total
    regs[dest]=regs[source1]+simmediate;
    
    //checks if the first added number is the same sign as the immediate
    if((regs[source1]&(0x80000000))==
       (simmediate&(0x8000))){
        
        //checks if the first added number is not the same sign as the total
        if((regs[source1]&(0x80000000))!=
           (regs[dest]&(0x80000000))){
            //there is overflow
            exit(-10);
        }
    }
}

void i_instruction::ADDIU(uint32_t *regs){
    //just adds and stores in register
    regs[dest]=regs[source1]+simmediate;
}

void i_instruction::SLTI(uint32_t *regs){
    int32_t temp=regs[source1];
    if(temp<simmediate){
        regs[dest]=1;
    }else {
        regs[dest]=0;
    }
}

void i_instruction::SLTIU(uint32_t *regs){
    //checks if it is less than if it is dest reg is set to 1 and 0 if not
    if(regs[source1]<simmediate){
        regs[dest]=1;
    }else {
        regs[dest]=0;
    }
}

void i_instruction::ANDI(uint32_t *regs){
    uint32_t temp = uimmediate<<16;
    
    //bitwise AND with immediate shifted left twice
    regs[dest]=regs[source1]&temp;
}

void i_instruction::ORI(uint32_t *regs){
    uint32_t temp = uimmediate<<16;
    
    //bitwise OR with immediate shifted left twice
    regs[dest]=regs[source1]|temp;
}

void i_instruction::XORI(uint32_t *regs){
    uint32_t temp = uimmediate<<16;
    
    //bitwise XOR with immediate shifted left twice
    regs[dest]=regs[source1]^temp;
}

void i_instruction::LUI(uint32_t *regs){
    //checks inst format
    if(regs[source1]==0){
        //shifts immediate and stores it
        regs[dest] = uimmediate<<16;
    } else {
        //-invalid instr format
        exit(-12);
    }
}

int i_instruction::LB(uint32_t *regs, uint32_t *data){
    uint32_t regvalue;
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    int returnval = 0;
    
    //uses load function for accessing data
    returnval = load(data, regvalue, 8, virtaddr);
    //if no error loads half-word into reg
    if(returnval==0){
        regs[dest]=regvalue;
        
        //if signed it makes all the higher bits 1
        if((regvalue&0x80)!=0){
            regs[dest]=(regs[dest]|0xFFFFFF00);
        }
    }
    return returnval;
}

int i_instruction::LH(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    uint32_t regvalue;
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //uses load function for accessing data
    returnval = load(data, regvalue, 16, virtaddr);
    //if no error loads byte into reg
    if(returnval==0){
        regs[dest]=regvalue;
        
        //if signed it makes all the higher bits 1
        if((regvalue&0x8000)!=0){
            regs[dest]=(regs[dest]|0xFFFF0000);
        }
    }
    return returnval;
}

int i_instruction::LWL(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    uint32_t regvalue;
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //loads the left half-word
    returnval = load(data, regvalue, 16, virtaddr);
    //if no error stores the half word in the two ms bytes
    if(returnval==0){
        regs[dest]=(regvalue<<16)+(regs[dest]&0x0000FFFF);
    }
    return returnval;
}

int i_instruction::LW(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    uint32_t regvalue;
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //uses load function for accessing data
    returnval = load(data, regvalue, 32, virtaddr);
    //if no error loads word into reg
    if(returnval==0){
        regs[dest]=regvalue;
    }
    return returnval;
}

int i_instruction::LBU(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    uint32_t regvalue;
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //uses load function for accessing data
    returnval = load(data, regvalue, 8, virtaddr);
    //if no error loads byte into reg
    if(returnval==0){
        regs[dest]=regvalue;
    }
    return returnval;
}

int i_instruction::LHU(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    uint32_t regvalue;
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //uses load function for accessing data
    returnval = load(data, regvalue, 16, virtaddr);
    //if no error loads half-word into reg
    if(returnval==0){
        regs[dest]=regvalue;
    }
    return returnval;
}

int i_instruction::LWR(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    uint32_t regvalue;
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //subtracts one to start the load from one byte to the left
    virtaddr -= 1;
    
    //uses load function for accessing data
    returnval = load(data, regvalue, 16, virtaddr);
    //if no error loads half-word into reg
    if(returnval==0){
        regs[dest]=regvalue+(regs[dest]&0xFFFF0000);
    }
    return returnval;
}

int i_instruction::SB(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    //retrives ls byte and ands it to only keep the ls byte
    uint32_t regvalue = (regs[dest]&0x000000FF);
    
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //uses store function
    returnval = store(data, regvalue, 8, virtaddr);
    
    return returnval;
}

int i_instruction::SH(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    //retrives ls half-word and ands it to only keep the ls half-word
    uint32_t regvalue = (regs[dest]&0x0000FFFF);
    
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //uses store function
    returnval = store(data, regvalue, 16, virtaddr);
    
    return returnval;
}

int i_instruction::SW(uint32_t *regs, uint32_t *data){
    int returnval = 0;
    //retrives word
    uint32_t regvalue = regs[dest];
    
    //address is sum of signed immediate and base which is then shifted left twice
    uint32_t virtaddr = simmediate + source1;
    virtaddr = virtaddr<<2;
    
    //uses store function
    returnval = store(data, regvalue, 32, virtaddr);
    
    return returnval;
}

int i_instruction::run(uint32_t *data, uint32_t *regs, uint32_t &pc, uint32_t &getc, uint32_t &putc){
    int returnval = 0;
    
    //chooses function
    switch(opcode){
        case 0x1:
            //multiple possible for this one
            if(regs[dest]==0){
                BLTZ(regs, pc);
            } else if(regs[dest]==1){
                BGEZ(regs, pc);
            } else if(regs[dest]==16){
                BLTZAL(regs, pc);
            } else if(regs[dest]==17){
                BGEZAL(regs, pc);
            } //if none choose error thrown
            else { returnval = -12; }
            break;
        case 0x4:
            BEQ(regs, pc);
            break;
        case 0x5:
            BNE(regs, pc);
            break;
        case 0x6:
            if(regs[dest]==0){
                BLEZ(regs, pc);
            } else { returnval = -12; }
            break;
        case 0x7:
            if(regs[dest]==0){
                BGTZ(regs, pc);
            } else { returnval = -12; }
            break;
        case 0x8:
            ADDI(regs);
            break;
        case 0x9:
            ADDIU(regs);
            break;
        case 0x0A:
            SLTI(regs);
            break;
        case 0x0B:
            SLTIU(regs);
            break;
        case 0x0C:
            ANDI(regs);
            break;
        case 0x0D:
            ORI(regs);
            break;
        case 0x0E:
            XORI(regs);
            break;
        case 0x0F:
            LUI(regs);
            break;
        case 0x20:
            returnval = LB(regs, data);
            break;
        case 0x21:
            returnval = LH(regs, data);
            break;
        case 0x22:
            returnval = LWL(regs, data);
            break;
        case 0x23:
            returnval = LW(regs, data);
            break;
        case 0x24:
            returnval = LBU(regs, data);
            break;
        case 0x25:
            returnval = LHU(regs, data);
            break;
        case 0x26:
            returnval = LWR(regs, data);
            break;
        case 0x28:
            returnval = SB(regs, data);
            break;
        case 0x29:
            returnval = SH(regs, data);
            break;
        case 0x2B:
            returnval = SW(regs, data);
            break;
        //if none choosen throws error
        default:
            //none choosen
            exit(-12);
    }
    //all inc pc
    pc_inc(pc);
    
    //ensures reg 0 is 0
    regs[0]=0;
    return returnval;
}