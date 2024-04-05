from abstract_system_constants import *
from abstract_system_alu import alu

#### Execute

def executeInstruction(instr,args, registers,systemState):
    if instr==LDR:
        (r1,addr,_)=args
        registers[r1] = systemState[addr]
    elif instr==STR:
        (r1,r2,_)=args
        addr2 = registers[r2]
        # print('Storing ',registers[r1],' in addr ',addr2)
        systemState[addr2]=registers[r1]
    elif instr==PUSH:
        sptr = registers[SP]
        (r1,_,_)=args
        systemState[sptr]=registers[r1]
        registers[SP]+=1    
    elif instr==POP:      
        sptr = registers[SP]    
        (r1,_,_)=args      
        registers[r1] = systemState[sptr]        
        registers[SP]-=1        
    elif instr==CBNZ:      
        (r1,addr,_)=args
        if registers[r1]!=0:
            registers[PC] = addr
    elif instr==CBZ:      
        (r1,addr,_)=args
        if registers[r1]==0:
            registers[PC] = addr
    elif instr==B:      
        pctr = registers[PC]    
        (_,sraddr,_)=args      
        registers[PC]=sraddr
    elif instr==BL:      
        pctr = registers[PC]    
        (_,sraddr,_)=args      
        registers[LR] = pctr
        registers[PC]=sraddr
    elif instr==BX:      
        (r1,_,_)=args      
        lreg = registers[r1]    
        registers[PC]=lreg        
    elif instr==MOV:
        (r1,r2,_)=args
        registers[r1]=registers[r2]
    elif instr==SET:      
        (r1,v2,_)=args
        registers[r1]=v2
    elif instr==NOP or instr==WFI:      
        (_,_,_)=args # do nothing
    elif instr==WFI:      
        registers[SCR]=1
    elif instr==HLI:
        (hl_instr,_,_)=args
        (systemState,registers) = hl_instr(systemState,registers)
    else:        
        (r3,r1,r2)=args
        registers[r3] = alu[instr](registers[r1],registers[r2]) 
    return (registers,systemState)        

