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
        # _=0
        # High-level instructions go here
        def fib_hl( systemState,registers ):
            # (r1,r2,r4)=(1,1,FIB_MAX)
            r1 = int(registers[1]) 
            r2 = int(registers[2])
            r3 = int(registers[2])
            r4 = int(registers[4])
            while r4!=0:
                r3=r1+r2
                r1=r2
                r2=r3
                r4-=1
                systemState[r4]=r3
            registers[1:5]=[r1,r2,r3,r4]
            return ( systemState,registers )
        def timerIRS(systemState,registers):
            print( 'IRS Called for Timer')
            return ( systemState,registers )
        loop = 1 # declare a label, the value is irrelevant as long as it's unique
        print(TIMER)
        fib_prog=[
        [SET,R1,10],
        [SET,R2,10], # start periodic timer
        [SET,R3,TIMER],
        [STR,R1,R3],
        [SET,R3,TIMER+1],
        [STR,R2,R3],
        [SET,R1,1],
        [SET,R2,1],
        [SET,R3,0],
        [SET,R4,FIB_MAX],
        # # [HLI,fib_hl], # example of how to mix high-level instructions with low-level ones
        [SET,R5,1],
        (loop,[ADD,R3,R1,R2]),
        [MOV,R1,R2],
        [MOV,R2,R3],
        [SUB,R4,R4,R5],
        [STR,R3,R4],
        [CBNZ,R4,loop],
        [WFI],
        [HLI, timerIRS ]
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
        if type(iw)==tuple:
            hliw,hli=iw
            # print("PC:",pc)
            hlCode[pc] = hli
            ramState[CODE+pc] = hliw
        else:
            ramState[CODE+pc] = iw
        pc+=1
        
    cfg=(PERIPHERALS,HL,hlCode)            
    return (ramState,cfg)

