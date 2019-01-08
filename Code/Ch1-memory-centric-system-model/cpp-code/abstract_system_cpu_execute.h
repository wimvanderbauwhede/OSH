#include <vector>
#include <tuple>
std::tuple<std::vector<uint64_t>&,std::vector<uint64_t>&> executeInstruction(uint64_t instr,std::tuple<uint64_t,uint64_t,uint64_t> args,std::vector<uint64_t>& registers,std::vector<uint64_t>& systemState);
