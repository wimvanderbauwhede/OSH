from abstract_system_constants import *
from abstract_system_cpu_irqs import checkIrqs
### Processor fetch-decode-execute operations 
from abstract_system_cpu_fetch import fetchInstruction
from abstract_system_cpu_decode import decodeInstruction
from abstract_system_cpu_execute import executeInstruction

### Processor action

def processorAction(systemState,irqs,registers,cfg):     
    (PERIPHERALS,HL,hlCode)=cfg
    if PERIPHERALS:
        ivt = systemState[IVT:IVTsz]
        (registers,irqs)=checkIrqs(registers,ivt,irqs)
    # fetch the instruction
    ir = fetchInstruction(registers,systemState,cfg)
    # Only do something if we are not waiting for an interrupt
    if not registers[SCR]: 
        if HL:
            (systemState,registers) = ir(systemState,registers)
        else:    
            # decode the instruction
            (instr,tup) = decodeInstruction(ir)    
            # execute the instruction
            (registers,systemState)= executeInstruction(instr,tup, registers,systemState)
    return (systemState,irqs,registers)  
