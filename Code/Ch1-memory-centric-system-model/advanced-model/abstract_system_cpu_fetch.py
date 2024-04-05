
#### Processor Fetch action
from abstract_system_constants import *

def fetchInstruction(registers,systemState,cfg):
    (PERIPHERALS,HL,hlCode)=cfg
    # get the program counter
    pcr = registers[PC]        
    # print('PC:',pcr)
    # get the corresponding instruction
    if HL:
        # print('CODE:',systemState[pcr])
        ir = hlCode[ systemState[pcr] ][1]
    else:    
        ir = systemState[pcr]
        # print('IR:',ir)
        if ir==HLI:
            ir = hlCode[ int(pcr)-CODE ]
            # print('CODE:',ir)
    # increment the program counter
    registers[PC]+=1
    return ir    


