#include <iostream>
#include <tuple>

#define True 1
#define False 0
/*
A model for a processor-based system. The main difference from the code in the book is that it uses NumPy arrays for performance.;
Furthermore,the model for loading high;
*/

#include "abstract_system_constants.h"
#include "abstract_system_peripherals.h"
#include "abstract_system_processor.h"
#include "abstract_system_code.h"

//#include "sys.h"

int main() {

    // Default settings
    const uint64_t     PRINT_CYCLES=False;
    const uint64_t     MAX_NCYCLES=512;
    // TODO: cmd line args

    // Initialise the state

    //systemState=[0]*systemStateSz
    std::vector<uint64_t> systemState;//(systemStateSz, 0);

    std::vector<uint64_t> timerState(timerStateSz,0);
    timerState[0]=MAX_NCYCLES+1;
    timerState[1]=MAX_NCYCLES+1;
    std::vector<uint64_t> kbdState(kbdStateSz,0);
    std::vector<uint64_t> nicState(nicStateSz,0);
    std::vector<uint64_t> ssdState(ssdStateSz,0);
    std::vector<uint64_t> gpuState(gpuStateSz,0);
    std::vector<uint64_t> ramState(ramStateSz);
    std::vector<uint64_t> irqs;
    uint64_t kbdIrq=0;
    uint64_t nicIrq=0;
    uint64_t ssdIrq=0;
    uint64_t gpuIrq=0;
    uint64_t timerIrq=0;    

    // Initialise the registers
    std::vector<uint64_t> registers(NREGS,0);//#[0]*16;
    // Set the program counter to the start of the code region
    registers[PC]=CODE;
    std::vector<uint64_t> ivt(IVTsz,CODE);

    // Initialise the intterupt vector table
    for (int idx = IVT; idx<IVT+IVTsz;idx++) {
        ramState[idx]=CODE;
    }

    ramState=load_code(ramState);

    int ct=0;
    for (auto elt : ramState ) {
        systemState.push_back(elt);
    }
    for (auto elt : timerState ) {
        systemState.push_back(elt);
    }
    for (auto elt : kbdState ) {
        systemState.push_back(elt);
    }
    for (auto elt : nicState ) {
        systemState.push_back(elt);
    }
    for (auto elt : ssdState ) {
        systemState.push_back(elt);
    }
    for (auto elt : gpuState ) {
        systemState.push_back(elt);
    }

    for (int ncycles = 0; ncycles<MAX_NCYCLES;ncycles++ ) {
        if (PRINT_CYCLES ) {
            std::cout << ncycles << "\n";
        }
#ifdef PERIPHERALS
        std::tie(kbdState,kbdIrq)=kbdAction(kbdState);
        std::tie(nicState,nicIrq)=nicAction(nicState);
        std::tie(ssdState,ssdIrq)=ssdAction(ssdState);
        std::tie(gpuState,gpuIrq)=gpuAction(gpuState);
        std::tie(timerState,timerIrq)=timerAction(timerState);
        std::vector<uint64_t>::const_iterator first = systemState.begin() ;
        std::vector<uint64_t>::const_iterator last = systemState.begin() + MEMTOP;
        std::vector<uint64_t> ramState(first,last);//=systemState[0:MEMTOP];

        irqs={kbdIrq,nicIrq,ssdIrq,gpuIrq,timerIrq};

        //                systemState=ramState+timerState+kbdState+nicState+ssdState+gpuState
        //systemState=concatenate((ramState,timerState,kbdState,nicState,ssdState,gpuState)); // FIXME
#else 
        irqs={};
        //            print(systemState[CODE:CODE+20])
#endif
        std::tie(systemState,irqs,registers)=processorAction(systemState,irqs,registers);

    }
    std::cout << IVT+IVTsz <<" .. " <<IVT+IVTsz+FIB_MAX<<"\n";
    for (int idx = IVT+IVTsz; idx<IVT+IVTsz+FIB_MAX;idx++) {
        std::cout <<idx<<"\t"<<systemState[idx]<<std::endl;
    }
//   std::cout <<systemState[IVT+IVTsz:FIB_MAX])<<std::endl;

}

