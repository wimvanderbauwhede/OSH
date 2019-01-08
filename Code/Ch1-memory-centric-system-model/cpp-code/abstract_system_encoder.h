#include <vector>
#include <tuple>
typedef std::vector< std::vector<uint64_t>> instrs_t;
uint64_t encodeInstruction(std::vector<uint64_t>& instr,std::vector<uint64_t>& labels);
std::vector<uint64_t> encodeProgram(instrs_t& instrs);
