uint64_t decode_instr(uint64_t ir);
std::tuple<uint64_t,uint64_t,uint64_t> decode_addr_arg(uint64_t ir);
std::tuple<uint64_t,uint64_t,uint64_t> decode_reg_addr_args(uint64_t ir);
std::tuple<uint64_t,uint64_t,uint64_t> decode_3reg_args(uint64_t ir);
std::tuple<uint64_t,std::tuple<uint64_t,uint64_t,uint64_t>> decodeInstruction(uint64_t ir);
