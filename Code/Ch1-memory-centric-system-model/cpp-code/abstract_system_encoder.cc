#ifdef VERBOSE
#include <iostream>
#endif
#include <map>
#include <vector>
#include <tuple>
#include "abstract_system_constants.h"
#include "abstract_system_encoder.h"

uint64_t encodeInstruction(std::vector<uint64_t>& instr,std::vector<uint64_t>& labels){
    uint64_t arg1;
    uint64_t arg1_;
    uint64_t arg2;
    uint64_t arg2_;
    uint64_t arg3;
    uint64_t iw;
    uint64_t opcode;
    uint64_t None=0;
    opcode=instr[1];
    arg1=None;
    arg2=None;
    arg3=None;
    if (opcode!=NOP and opcode!=WFI ) {
        arg1=instr[2];
        if (opcode!=BL and opcode!=B and opcode!=BX ) {
            arg2=instr[3];
            if (opcode!=LDR and opcode!=STR and opcode!=CBZ and opcode!=CBNZ and opcode!=MOV and opcode!=SET ) {
//                print(instr)
//                print(opcode)
                arg3=instr[4];
            }//16 > 4
        }//16 > 4
    }//16 > 4
    iw=None;
    if (opcode==BL or opcode==B  ) {
        arg1_=labels[arg1] + CODE;
        iw=opcode+(arg1_<<8);
     } else if (opcode==CBZ or opcode==ADR or opcode==CBNZ ) {
        arg2_=labels[arg2] + CODE;
        iw=opcode+(arg1<<8)+(arg2_<<16);
     } else if (opcode==MOV or opcode==SET or opcode==LDR or opcode==STR ) {
        iw=opcode+(arg1<<8)+(arg2<<16);
     } else if (opcode==NOP or opcode==WFI ) {
        iw=opcode;
     } else  {
//        print(opcode,arg1,arg2,arg3)
        iw=opcode+(arg1<<8)+(arg2<<16)+(arg3<<24);
    }//8 > 4
    return iw;

}//4 > 0

std::vector<uint64_t> encodeProgram(instrs_t& instrs){
    uint64_t encoded_instr;
    std::vector<uint64_t> encoded_instrs;
    //std::vector<uint64_t> instr;
    std::vector<uint64_t> labels(instrs.size(),0);
    uint64_t pc;
    uint64_t None=0;
    pc=0;
    labels.clear();//={};
    encoded_instrs.clear();//=[];
    for (auto instr : instrs ) {
        //std::cout << instr.size() << ";"<<instr[1]<<"\n";
        encoded_instr=None;
        if (instr[0]!=0 ) {
#ifdef VERBOSE
            std::cout << "label "<<instr[0]<<" on line "<<pc<<"\n";
#endif            
            labels[instr[0]]=pc;
            encoded_instr=encodeInstruction(instr,labels);
         } else {
            encoded_instr=encodeInstruction(instr,labels);
//print(pc,encoded_instr)
        }//12 > 8
        encoded_instrs.push_back(encoded_instr);
        pc+=1;
    }//8 > 4
    return encoded_instrs;
}
