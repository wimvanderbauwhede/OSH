from abstract_system_constants import *

def encodeInstruction(instr,labels):
    opcode=instr[0]
    arg1=None
    arg2=None
    arg3=None
    if opcode!=NOP and opcode!=WFI:
        arg1=instr[1]
        if opcode!=BL and opcode!=B and opcode!=BX:    
            arg2=instr[2]
            if opcode!=LDR and opcode!=STR and opcode!=CBZ and opcode!=CBNZ and opcode!=MOV and opcode!=SET:
#                print(instr)
#                print(opcode)
                arg3=instr[3]
    iw=None                
    if opcode==BL or opcode==B :
        arg1_ = labels[arg1] + CODE
        iw = opcode+(arg1_<<8)
    elif opcode==CBZ or opcode==ADR or opcode==CBNZ:
        arg2_ = labels[arg2] + CODE
        iw = opcode+(arg1<<8)+(arg2_<<16)
    elif opcode==MOV or opcode==SET or opcode==LDR or opcode==STR:
        iw=opcode+(arg1<<8)+(arg2<<16)
    elif opcode==NOP or opcode==WFI:
        iw = opcode
    else :    
#        print(opcode,arg1,arg2,arg3)
        iw = opcode+(arg1<<8)+(arg2<<16)+(arg3<<24)
    return iw

def encodeProgram(instrs):
    pc=0
    labels={}
    encoded_instrs=[]
    for instr in instrs:
        encoded_instr=None
        if type(instr)==tuple:
            #print('label ',instr[0],' on line ',pc)
            labels[instr[0]]=pc
            instr_=instr[1]
            encoded_instr=encodeInstruction(instr_,labels)
        else:    
            encoded_instr=encodeInstruction(instr,labels)
        #print(pc,encoded_instr)    
        encoded_instrs.append(encoded_instr)
        pc+=1
    return encoded_instrs
