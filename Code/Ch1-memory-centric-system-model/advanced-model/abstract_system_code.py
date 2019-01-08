from abstract_system_constants import *
from abstract_system_encoder import *

FIB_MAX=84
def load_code(ramState,cfg):
    (PERIPHERALS,HL,hlCode)=cfg
    if HL:
        def fib_hl( systemState,registers ):
            (r1,r2,r4)=(1,1,FIB_MAX)
            while r4!=0:
                r3=r1+r2
                r1=r2
                r2=r3
                r4-=1
                systemState[r4]=r3
            registers[1:5]=[r1,r2,r3,r4]        
            return ( systemState,registers )  
#        hlCode[0]=fib_hl
        fib_prog=[[HLI,fib_hl]]
#        ramState[CODE]=0
    else:
        _=0
        loop=1
        fib_prog=[
        [SET,R1,1],
        [SET,R2,1],
        [SET,R3,0],
        [SET,R4,FIB_MAX],
        [SET,R5,1],
        (loop,[ADD,R3,R1,R2]),
        [MOV,R1,R2],
        [MOV,R2,R3],
        [SUB,R4,R4,R5],
        [STR,R3,R4],
        [CBNZ,R4,loop],
        [WFI]
        ]
        
    #print(fib_prog)
    fib_iws=encodeProgram(fib_prog)
#        pc=0
#        for iw in fib_iws:
#            decoded_instr = decodeInstruction(iw)
#            print(pc,decoded_instr)
#            pc+=1

    pc=0
    for iw in fib_iws:
        ramState[CODE+pc] = iw  
        pc+=1
        
    cfg=(PERIPHERALS,HL,hlCode)            
    return (ramState,cfg)

