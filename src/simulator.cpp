#include <cstdlib>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdint>
#include <vector>
#include "r_instruction.hpp"

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
        if(!myfile.eof()){
            instructions.push_back(instuctionline);
        }
    }
    //stops at the end of the file
    
    //returns the instructions vector
    return instructions;
}

int getfunc_type(string instruction){
    int opcode=0;
    for(int i=0; i<6;i++){
        if(instruction[i]=='1'){
            opcode+=pow(2,5-i);
        }
    }
    
    if(opcode==0){ return 0;}
    else if(opcode==2||opcode==3){ return 2;}
    else{ return 1;}
    
}


int main()
{
    //declares the memory elements we will use
    int data_size = 1;
    uint32_t regs[32];
    regs[1]=0x80000000;
    regs[2]=0x80000000;
    uint32_t* data[data_size];
    uint32_t pc=0;
    
    //converts the text file of instructions into a more managable vector of strings of instructions
    vector<string> inst_vector;
    string filename = "textfile.txt";
    inst_vector = make_instuction_vector(filename);
    int function_type=-1;
    
    for(int i=0; i<inst_vector.size(); i++){
        function_type = getfunc_type(inst_vector[i]);
        if(function_type==0){
            r_instruction rinst(inst_vector[i]);
            int a = rinst.run(regs, pc);
            cout<<"_____"<< pc<<endl;
        }
    }
    
    
    return 0;
}
