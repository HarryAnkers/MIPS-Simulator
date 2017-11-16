#include <cstdlib>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdint>
#include <vector>
#include "r_instruction.hpp"
#include "i_instruction.hpp"
#include "j_instruction.hpp"

using namespace std;

//function to convert the file of binary instructions to a string of char "1" and "0"
vector<string> make_instuction_vector(string filename){
    //declares fstream
    ifstream myfile;
    myfile.open(filename);
    
    //declares a temp vector of instructions and a 32 char string for each line
    vector<string> instructions;
    char instuctionline[32];
    
    //puts 32 char into the instruction vector
    while(!myfile.eof()){
        for(int i=0; i<32; i++){
            myfile>>instuctionline[i];
        }
        if(!myfile.eof()&&(instructions.size()<0x1000000)){
            instructions.push_back(instuctionline);
        } else if(instructions.size()<0x1000000){
            instructions.push_back("too many instructions");
        }
    }
    //stops at the end of the file
    
    //returns the instructions vector
    return instructions;
}

char getfunc_type(string instruction){
    int opcode=0;
    for(int i=0; i<6;i++){
        if(instruction[i]=='1'){
            opcode+=pow(2,5-i);
        }
    }
    
    if(opcode==0){ return 'r';}
    else if(opcode==2||opcode==3){ return 'j';}
    else{ return 'i';}
    
}


int main()
{
    //declares the memory elements we will use
    uint32_t regs[32];
    regs[1]=0x80000000;
    regs[2]=0x80000000;
    uint32_t* data = new uint32_t [0x4000000]();
    uint32_t pc=0;
    uint32_t getc, putc, HI, LO;
    
    //converts the text file of instructions into a more managable vector of strings of instructions
    string filename = "textfile.txt";
    vector<string> inst_vector = make_instuction_vector(filename);
    if(inst_vector.size()!=0x1000000){
        char function_type='0';
       
        function_type = getfunc_type(inst_vector[0+(pc/4)]);
        if(function_type=='r'){
            r_instruction rinst(inst_vector[0+(pc/4)]);
            int a = rinst.run(regs, pc);
        } else if(function_type=='i'){
            i_instruction iinst(inst_vector[0+(pc/4)]);
            int a = iinst.run(regs, pc);
        } else {
            j_instruction jinst(inst_vector[0+(pc/4)]);
            int a = jinst.run(pc);
        }
                                     
    } else {
        cout<<"too many instructions for memory"<<endl;
    }
    
    uint32_t b =0xFFFFFFFF;
    b=b>>28;
    b=b<<28;
    cout<<b;
    
    return 0;
}
