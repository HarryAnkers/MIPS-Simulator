 #!/bin/bash
g++ -std=c++11 simulator.cpp r_instruction.cpp i_instruction.cpp j_instruction.cpp -o test
./test 	textfile.txt
echo pass1

