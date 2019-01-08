#include <vector>
#include <tuple>
std::tuple<std::vector<uint64_t>&,std::vector<uint64_t>&> checkIrqs(std::vector<uint64_t>& registers,std::vector<uint64_t>& ivt,std::vector<uint64_t>& irqs);
