### Processor fetch-decode-execute operations 
from abstract_system_cpu_fetch import fetchInstruction
from abstract_system_cpu_decode import decodeInstruction
from abstract_system_cpu_execute import executeInstruction

### Processor action

def processorAction(systemState,registers): 
    # fetch the instruction
    ir = fetchInstruction(registers,systemState)
    # decode the instruction
    (instr,args) = decodeInstruction(ir)
    # execute the instruction
    (registers,systemState)= executeInstruction(instr,args,registers,systemState)
    return (systemState,registers)  


