from abstract_system_constants import *
from abstract_system_alu import alu

def  doLDR(registers,systemState,args):
       (r1,addr,_)=args
       registers[r1] = systemState[addr]
       return (registers,systemState)
       
def doSTR(registers,systemState,args):       
       (r1,addr,_)=args
       systemState[addr]=registers[r1]
       return (registers,systemState)
                       
def doB(registers,args):
       (_,addr,_)=args
       registers[PC] = addr
       return registers

def doCBZ(registers,args):
       (r1,addr1,addr2)=args
       if registers[r1]:
            registers[PC] = addr1
       else:
            registers[PC] = addr2
       return registers
       
def doALU(instr,registers,args):
       (r1,r2,r3)=args
       registers[r1] = alu[instr](registers[r2],registers[r3]) 
       return registers

#### Execute
def executeInstruction(instr,args, registers,systemState):
    if instr==LDR: 
        (registers,systemState) = doLDR(args, registers,systemState)
    elif instr==STR:
        (registers,systemState) = doSTR(registers,systemState,args)       
    elif instr==B:
        registers = doB(registers,args)
    elif instr==CBZ:      
        registers = doCBZ(registers,args)
    else:            
        registers = doALU(instr,registers,args)        
    return (registers,systemState)        

