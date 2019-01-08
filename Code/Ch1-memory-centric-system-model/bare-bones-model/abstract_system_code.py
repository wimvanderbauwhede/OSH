from abstract_system_constants import *
from abstract_system_encoder import *

FIB_MAX=84
def load_code(ramState):
    mul_prog=[
    [MUL,R3,R1,R2]            
    ]
        
    mul_iws=encodeProgram(mul_prog)

    pc=0
    for iw in mul_iws:
        ramState[CODE+pc] = iw  
        pc+=1
        
    return ramState

