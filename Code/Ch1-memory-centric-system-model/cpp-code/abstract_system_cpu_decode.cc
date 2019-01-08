#include <tuple>
#include "abstract_system_constants.h"
//### Decode

uint64_t decode_instr(uint64_t ir){
    uint64_t None=0;
// The instruction code is the first byte
//return  ir & 0xFF
    return  ir & 0xFF;

}//4 > 0

std::tuple<uint64_t,uint64_t,uint64_t> decode_addr_arg(uint64_t ir){
    uint64_t addr;
    uint64_t None=0;
// load and store use the next 2 bytes
// for the register address and the memory address
    addr=ir >>  8;
//addr=ir >>  8
// return as tuple
    return std::tie(addr,None,None);

}//4 > 0

std::tuple<uint64_t,uint64_t,uint64_t> decode_reg_addr_args(uint64_t ir){
    uint64_t addr;
    uint64_t r1;
    uint64_t None=0;
// load and store use the next 2 bytes
// for the register address and the memory address
    r1=(ir >>  8)&  0xFF;
    addr=ir >>  16;
//r1=(ir >>  8)&  0xFF
//addr=ir >>  16
// return as tuple
    return std::tie(r1,addr,None);

}//4 > 0

std::tuple<uint64_t,uint64_t,uint64_t> decode_3reg_args(uint64_t ir){
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
    uint64_t None=0;
// all other instructions use the next 3 bytes
// for the register addresses
    r1=(ir >>  8)&  0xFF;
    r2=(ir >>  16)&  0xFF;
    r3=(ir >>  24)&  0xFF;
//r1=(ir >>  8)&  0xFF
//r2=(ir >>  16)&  0xFF
//r3=(ir >>  24)&  0xFF
// return as tuple
    return std::tie(r1,r2,r3);

}//4 > 0

std::tuple<uint64_t,std::tuple<uint64_t,uint64_t,uint64_t>> decodeInstruction(uint64_t ir){
    uint64_t instr;
    std::tuple<uint64_t,uint64_t,uint64_t> tup;
    uint64_t None=0;
    instr=decode_instr(ir);
    if (instr==LDR or instr==STR or instr==MOV or instr==SET or instr==CBZ or instr==CBNZ ) {
        tup=decode_reg_addr_args(ir);
     } else if (instr==B or instr==BL or instr==BX ) {
        tup=decode_addr_arg(ir);
     } else if (instr==NOP or instr==WFI ) {
        tup= std::make_tuple(None,None,None);
     } else {
        tup=decode_3reg_args(ir);
// return as tuple
    }//8 > 4
    return std::tie(instr,tup);
}
