#include <vector>
#include <tuple>
std::tuple<std::vector<uint64_t>,uint64_t> kbdAction(std::vector<uint64_t>& kbdState);
std::tuple<std::vector<uint64_t>,uint64_t> nicAction(std::vector<uint64_t>& nicState);
std::tuple<std::vector<uint64_t>,uint64_t> ssdAction(std::vector<uint64_t>& ssdState);
std::tuple<std::vector<uint64_t>,uint64_t> gpuAction(std::vector<uint64_t>& gpuState);
std::tuple<std::vector<uint64_t>,uint64_t> timerAction(std::vector<uint64_t>& timerState);
