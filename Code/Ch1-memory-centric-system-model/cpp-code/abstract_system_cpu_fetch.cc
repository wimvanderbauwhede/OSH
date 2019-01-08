#ifdef VERBOSE
#include <iostream>
#endif
#include <vector>
#include <tuple>

//### Processor Fetch action
#include "abstract_system_constants.h"

uint64_t fetchInstruction(std::vector<uint64_t>& registers,std::vector<uint64_t>& systemState){
    
// get the program counter
    uint64_t pcr=registers[PC];
#ifdef VERBOSE
    std::cout <<"fetch PC: "<<pcr<<"\n";
#endif
// get the corresponding instruction
    uint64_t ir=systemState[pcr];
#ifdef VERBOSE
    std::cout <<"fetch IR: "<<ir<<"\n";
#endif
//        print('CODE:',ir)
// increment the program counter
    registers[PC]+=1;
    return ir;


}
