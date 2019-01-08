#include <tuple>
#ifdef VERBOSE
#include <iostream>
#endif
#include "abstract_system_constants.h"
#include "abstract_system_decoder.h"
#include "abstract_system_encoder.h"
#include "abstract_system_code.h"

std::vector<uint64_t>& load_code(std::vector<uint64_t>& ramState){
    uint64_t _ = 0;
    uint64_t loop = 1;

    std::vector< std::vector<uint64_t>> fib_prog={
        {_,SET,R1,1,_},
        {_,SET,R2,1,_},
        {_,SET,R3,0,_},
        {_,SET,R4,FIB_MAX,_},
        {_,SET,R5,1,_},
        {loop,ADD,R3,R1,R2},
        {_,MOV,R1,R2,_},
        {_,MOV,R2,R3,_},
        {_,SUB,R4,R4,R5},
        {_,STR,R3,R4,_},
        {_,CBNZ,R4,loop,_},
        {_,WFI,_,_,_}
    };
    std::vector<uint64_t>   fib_iws=encodeProgram(fib_prog);
    uint64_t pc=0;
    for (auto iw : fib_iws) {
#ifdef VERBOSE        
        std::cout << pc<<" : "<<iw << "\n";
#endif
        std::tuple<uint64_t,std::tuple<uint64_t,uint64_t,uint64_t>> decoded_instr=decodeInstruction(iw);
        uint64_t v1;
        std::tuple<uint64_t,uint64_t,uint64_t> t2;
        uint64_t v2,v3,v4;
        std::tie(v1,t2)=decoded_instr;
        std::tie(v2,v3,v4)=t2;
#ifdef VERBOSE
        std::cout << pc<<" : " <<  v1 << ";" << v2 << "," <<  v3<< "," <<  v4 << "\n";
#endif
        pc+=1;
    }
#ifdef VERBOSE
    std::cout << "CODE: "<<CODE<<"\n";
#endif
    pc=0;
    for (uint64_t iw : fib_iws ) {
#ifdef VERBOSE
        std::cout << "write IW " << iw << " into " << CODE+pc << "\n";
#endif
        ramState[CODE+pc]=iw;
        pc+=1;
    }
    return ramState;
}
