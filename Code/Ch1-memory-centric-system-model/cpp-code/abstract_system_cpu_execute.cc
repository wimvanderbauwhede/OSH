#ifdef VERBOSE
#include <iostream>
#endif
#include <tuple>
#include <vector>
#include "abstract_system_constants.h"
#include "abstract_system_alu.h"

//### Execute

std::tuple<std::vector<uint64_t>&,std::vector<uint64_t>&> executeInstruction(uint64_t instr,std::tuple<uint64_t,uint64_t,uint64_t> args,std::vector<uint64_t>& registers,std::vector<uint64_t>& systemState){
    uint64_t _;
    uint64_t addr;
    uint64_t addr2;
//    std::tuple<uint64_t,uint64_t,uint64_t> args;
    uint64_t lreg;
    uint64_t pctr;
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
    uint64_t sptr;
    uint64_t sraddr;
    uint64_t v2;
    uint64_t None=0;
    if (instr==LDR ) {
        std::tie(r1,addr,_)=args;
        registers[r1]=systemState[addr];
     } else if (instr==STR ) {
        std::tie(r1,r2,_)=args;
        addr2=registers[r2];
#ifdef VERBOSE
        std::cout <<"Storing "<<registers[r1]<<" in addr "<<addr2<<"\n";
#endif        
        systemState[addr2]=registers[r1];
     } else if (instr==PUSH ) {
        sptr=registers[SP];
        std::tie(r1,_,_)=args;
        systemState[sptr]=registers[r1];
        registers[SP]+=1;
     } else if (instr==POP ) {
        sptr=registers[SP];
        std::tie(r1,_,_)=args;
        registers[r1]=systemState[sptr];
        registers[SP]-=1;
     } else if (instr==CBNZ ) {
        std::tie(r1,addr,_)=args;
        if (registers[r1]!=0 ) {
            registers[PC]=addr;
    }//12 > 4
     } else if (instr==CBZ ) {
        std::tie(r1,addr,_)=args;
        if (registers[r1]==0 ) {
            registers[PC]=addr;
    }//12 > 4
     } else if (instr==BL ) {
        pctr=registers[PC];
        std::tie(_,sraddr,_)=args;
        registers[LR]=pctr;
        registers[PC]=sraddr;
     } else if (instr==BX ) {
        std::tie(r1,_,_)=args;
        lreg=registers[r1];
        registers[PC]=lreg;
     } else if (instr==MOV ) {
        std::tie(r1,r2,_)=args;
        registers[r1]=registers[r2];
     } else if (instr==SET ) {
        std::tie(r1,v2,_)=args;
        registers[r1]=v2;
     } else if (instr==NOP or instr==WFI ) {
        std::tie(_,_,_)=args;
//    elif instr==WFI:
//        exit()# FIXME,this should wait for an interrupt instead
     } else {
        std::tie(r3,r1,r2)=args;
        uint64_t v1 = registers[r1];
        uint64_t v2 = registers[r2];
        uint64_t v3 = alu[instr](v1,v2);
        registers[r3] = v3;
    }//8 > 4
    return std::tie(registers,systemState);

}
