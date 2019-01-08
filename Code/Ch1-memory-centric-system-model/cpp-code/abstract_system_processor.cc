#ifdef VERBOSE
#include <iostream>
#endif
#include <vector>
#include <tuple>
#include "abstract_system_constants.h"
#include "abstract_system_cpu_irqs.h"
//## Processor fetch-decode-execute operations
#include "abstract_system_cpu_fetch.h"
#include "abstract_system_cpu_decode.h"
#include "abstract_system_cpu_execute.h"

//## Processor action

std::tuple<std::vector<uint64_t>&,std::vector<uint64_t>& ,std::vector<uint64_t>& > processorAction(std::vector<uint64_t>& systemState,std::vector<uint64_t>&  irqs,std::vector<uint64_t>&  registers){
    uint64_t instr;
    std::tuple<uint64_t,uint64_t,uint64_t> tup;
#ifdef PERIPHERALS 
        std::vector<uint64_t>::const_iterator first = systemState.begin() + IVT;
        std::vector<uint64_t>::const_iterator last = systemState.begin() + IVT+IVTsz;
        std::vector<uint64_t> ivt(first, last);
        std::tie(registers,irqs)=checkIrqs(registers,ivt,irqs);
#endif        
// fetch the instruction
    uint64_t ir=fetchInstruction(registers,systemState);
#ifdef VERBOSE
    std::cout << "proc IR: "<<ir<<"\n";
#endif
// decode the instruction
    std::tie(instr,tup)=decodeInstruction(ir);
    uint64_t t1;uint64_t t2;uint64_t t3;
    std::tie(t1,t2,t3)=tup;
#ifdef VERBOSE
    std::cout << "proc INSTR: "<<instr<<" TUP: ("<< t1<<","<<t2<<","<<t3<<")\n";
#endif
//        print(instr,tup)
// execute the instruction
    std::tie(registers,systemState)=executeInstruction(instr,tup,registers,systemState);
#ifdef VERBOSE
    for (int i=0;i<NREGS;i++ ) {
    std::cout << "proc REG: "<<registers[i] << "\n";
    }
#endif
//print(registers)
    return std::tie(systemState,irqs,registers);
}
