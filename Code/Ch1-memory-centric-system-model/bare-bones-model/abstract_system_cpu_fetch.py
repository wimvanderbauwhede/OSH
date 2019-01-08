#### Processor Fetch action
from abstract_system_constants import *

def fetchInstruction(registers,systemState):
    # get the program counter
    pcr = registers[PC]        
    # get the corresponding instruction
    ir = systemState[pcr]            
    # increment the program counter
    registers[PC]+=1
    return ir    


