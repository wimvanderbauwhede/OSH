from abstract_system_constants import *
#### Decode 

def decode_instr(ir): 
    # The instruction code is the first byte
    return  ir & 0xFF

def decode_addr_arg(ir): 
    # load and store use the next 2 bytes 
    # for the register address and the memory address
    addr =  ir >>  8
    # return as tuple
    return (addr,None,None)

def decode_reg_addr_args(ir): 
    # load and store use the next 2 bytes 
    # for the register address and the memory address
    r1= ( (ir) >>  (8)) &  (0xFF)
    addr =  (ir) >>  (16)
    # return as tuple
    return (r1,addr,None)

def decode_3reg_args(ir): 
    # all other instructions use the next 3 bytes 
    # for the register addresses
    r1 =  ( (ir) >>  (8)) &  (0xFF)
    r2 =  ( (ir) >>  (16)) &  (0xFF)
    r3 =  ( (ir) >>  (24)) &  (0xFF)
    # return as tuple    
    return (r1,r2,r3)

def decodeInstruction(ir):
    instr = decode_instr(ir)
    if instr==LDR or instr==STR or instr==MOV or instr==SET or instr==CBZ or instr==CBNZ:
        tup=decode_reg_addr_args(ir)
    elif instr==B or instr==BL or instr==BX:
        tup = decode_addr_arg(ir)
    elif instr==NOP or instr==WFI:
        tup=(None,None,None)
    else:
        tup=decode_3reg_args(ir)
    # return as tuple        
    return (instr,tup)
