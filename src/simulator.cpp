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
#include <cmath>
uint32_t RAM[datasize]={0};
vector<uint32_t> ROM;

using namespace std;

//function to convert the file of binary instructions to a string of char "1" and "0"
void make_instuction_vector(string filename, int &error){
    //declares a uint8 for each byte and a uint32 for each line
    uint8_t instbyte=0;
    uint32_t instline;
    
    //declares fstream
    ifstream myfile;
    myfile.open(filename);
    if(myfile.is_open()){
        
        //puts 32 char into the instruction vector
        while(!myfile.eof()){
            instline=0;
            for(int i=0; i<4; i++){
                myfile>>instbyte;
                instline += instbyte*pow(2, (8*(3-i)));
            }
            
            if(!myfile.eof()){
                ROM.push_back(instline);
            }
        }
        //stops at the end of the file
        
        if(ROM.size()>instsize){
            error = 2;
        }
    } else { error = 1; }
}

char getfunc_type(uint32_t instruction){
    int opcode=instruction>>24;
    
    if(opcode==0){ return 'r';}
    else if(opcode==2||opcode==3){ return 'j';}
    else{ return 'i';}
}


int main(int argc, const char * argv[])
{
    //declares the memory elements we will use except the ROM and RAM which is global
    uint32_t regs[32] = {0};
    
    uint32_t pc=0;
    uint32_t getc, putc, HI, LO;
    int error = 0;
    bool done = false;
    
    //converts the text file of instructions into a more managable vector of uint32 instructions
    //string filename = argv(1)
    make_instuction_vector(argv[1], error);
    
    
    while(done == false){
        if(error==0){
            //checks the opcode and creates a class accordingly
            char function_type='0';
           
            function_type = getfunc_type(ROM[0+(pc/4)]);
            if(function_type=='r'){
                r_instruction rinst(ROM[0+(pc/4)]);
                error = rinst.run(HI, LO, regs, pc);
            } else if(function_type=='i'){
                i_instruction iinst(ROM[0+(pc/4)]);
                error = iinst.run(RAM, regs, pc);
            } else {
                j_instruction jinst(ROM[0+(pc/4)]);
                error = jinst.run(regs, pc);
            }
            
            if((error != 0)&&(error != -1)){
                cout<< "error: "<<error<<endl;
            } else if( error ==-1){
                //bool to finish
                done = true;
            } else if(error == 1) {
                cout<<"error: file could not open"<<endl;
            } else if(error == 2) {
                cout<<"error: too many instructions"<<endl;
            } else if(error == 3) {
                //need to add get c func
            }
        }
        cout<<"program executed"<<endl<<endl;
        return 0;
    }
}
