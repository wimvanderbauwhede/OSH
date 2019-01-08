#include <vector>
#include <tuple>
//## Processor interrupt handling
#include "abstract_system_constants.h"
const uint64_t False = 0;
std::tuple<std::vector<uint64_t>&,std::vector<uint64_t>&> checkIrqs(std::vector<uint64_t>& registers,std::vector<uint64_t>& ivt,std::vector<uint64_t>& irqs){
    uint64_t idx;
    uint64_t None=0;
    idx=0;
    for (auto irq : irqs ) {
        if (irq  ) {
//print("IRQ ",idx," raised!")
// Save the program counter in the link register
            registers[LR]=registers[PC];
// Set program counter to ISR start address
            registers[PC]=ivt[idx];
            irqs[idx]=False;
            break;
        }//12 > 8
        idx+=1;
    }//8 > 4
    return std::tie(registers,irqs);

}
