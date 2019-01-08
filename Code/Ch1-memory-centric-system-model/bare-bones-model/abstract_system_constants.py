systemStateSz=0x1000000 # 16MB
timerStateSz=0x10 # 16
kbdStateSz=0x100 # 256B
nicStateSz=0x1000 # 4KB
ssdStateSz=0x100000 # 1MB
gpuStateSz=0x80000 # 512KB
ramStateSz=0xE00000 # 14MB

# High-level code array size
hlCodeSz=0x1000

# RAM organisation
IVTsz=16
heapSz=0x800000
stackSz=0x400000
codeSz=0x200000

# OPCODES
"""
#Thumb:
#ALU = (16 << 4) + 4-bit opcode
# opcode =
AND=0
EOR=1
LSL=2
LSR=3
ASR=4
ADC=5
SBC=6
ROR=7
TST=8
NEG=9
CMP=10
CMN=11
OR=12
MUL=13
BIC=14
MVN=15

# ADD/SUB 12 << 4 + 1-bit opcode << 3 

ADD = 0
SUB = 1
"""
ADD=0 #Thumb
SUB=1 #Thumb
MUL=2 #Thumb
DIV=3
AND=4 #Thumb
ORR=5 #Thumb
EOR=6 #Thumb
ASR=7 #Thumb
ASL=8
LDR=16 #Thumb
STR=17 #Thumb
BL=18 #Thumb
BX=19 #Thumb
PUSH=20 #Thumb 1011 0 10 1 00
POP=21 #Thumb 1011 1 10 1 00
MOV=22 #Thumb
SET=23
NOP=24 #Thumb
CBZ=25
CBNZ=26
B=27 #Thumb
ADR=28 #Thumb
WFI=29
HLI=30
MSR=31 # Move the contents of a general-purpose register into the specified special register.

#MSR{cond} spec_reg, Rn
#spec_reg can be any of: APSR, IPSR, EPSR, IEPSR, IAPSR, EAPSR, PSR, MSP, PSP, PRIMASK, BASEPRI, BASEPRI_MAX, FAULTMASK, or CONTROL.


# REGISTERS
R0=0
R1=1
R2=2
R3=3
R4=4
R5=5
R6=6
R7=7
R8=8
R9=9
SB=9
R10=10
SL=10
R11=11
FP=11
R12=12
SP=13
LR=14
PC=15

PSR=16 # Program status register
PRIMASK=17 # priority mask
FAULTMASK=18 # fault mask
BASEPRI=19 # base priority mask
CONTROL=20 # Control register, used to select the stack: 
#    0 = Main Stack Pointer (MSP). This is the reset value.
#    1 = Process Stack Pointer (PSP).
MSP=21
PSP=22

# Memory Map, for the actual memory
# Lower 8MB is heap
# next 4M is stack
# next 2MB is code
IVT=0x0
HEAP=IVT+IVTsz
STACK=HEAP+heapSz
CODE=STACK+stackSz
MEMTOP=CODE+codeSz
TIMER=MEMTOP
KBD=TIMER+timerStateSz
NIC=KBD+kbdStateSz
SSD=NIC+nicStateSz
GPU=SSD+ssdStateSz
STATETOP=GPU+gpuStateSz

