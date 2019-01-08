#include <vector>
#include <tuple>
//## Peripheral actions
#define True 1
#define False 0

std::tuple<std::vector<uint64_t>,uint64_t> kbdAction(std::vector<uint64_t>& kbdState){
    uint64_t kbdIrq;
    uint64_t None=0;
    kbdIrq=False;
    return std::make_tuple(kbdState,kbdIrq);

}//4 > 0

std::tuple<std::vector<uint64_t>,uint64_t> nicAction(std::vector<uint64_t>& nicState){
    uint64_t nicIrq;
    uint64_t None=0;
    nicIrq=False;
    return std::make_tuple(nicState,nicIrq);

}//4 > 0

std::tuple<std::vector<uint64_t>,uint64_t> ssdAction(std::vector<uint64_t>& ssdState){
    uint64_t ssdIrq;
    uint64_t None=0;
    ssdIrq=False;
    return std::make_tuple(ssdState,ssdIrq);

}//4 > 0

std::tuple<std::vector<uint64_t>,uint64_t> gpuAction(std::vector<uint64_t>& gpuState){
    uint64_t gpuIrq;
    uint64_t None=0;
    gpuIrq=False;
    return std::make_tuple(gpuState,gpuIrq);

}//4 > 0

std::tuple<std::vector<uint64_t>,uint64_t> timerAction(std::vector<uint64_t>& timerState){
    uint64_t timerIrq;
    uint64_t None=0;
    if (timerState[0]>0 ) {
        timerState[0]-=1;
    }//8 > 4
    timerIrq=False;
    if (timerState[0]==0 ) {
        timerIrq=True;
        timerState[0]=timerState[1];
    }//8 > 4
    return std::make_tuple(timerState,timerIrq);
}
