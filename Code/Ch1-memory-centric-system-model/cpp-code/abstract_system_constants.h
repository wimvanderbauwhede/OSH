#ifndef __ABSTRACT_SYSTEM_CONSTANTS__
#define __ABSTRACT_SYSTEM_CONSTANTS__
//#include <tuple>
const uint64_t systemStateSz=0x1000000;
const uint64_t timerStateSz=0x10;
const uint64_t kbdStateSz=0x100;
const uint64_t nicStateSz=0x1000;
const uint64_t ssdStateSz=0x100000;
const uint64_t gpuStateSz=0x80000;
const uint64_t ramStateSz=0xE00000;

// High-level code array size
const uint64_t hlCodeSz=0x1000;

// RAM organisation
const uint64_t IVTsz=16;
const uint64_t heapSz=0x800000;
const uint64_t stackSz=0x400000;
const uint64_t codeSz=0x200000;

// OPCODES

const uint64_t ADD=0;
const uint64_t SUB=1;
const uint64_t MUL=2;
const uint64_t DIV=3;
const uint64_t AND=4;
const uint64_t OR=5;
const uint64_t EOR=6;
const uint64_t ARS=7;
const uint64_t ALS=8;

const uint64_t LDR=16;
const uint64_t STR=17;
const uint64_t BL=18;
const uint64_t BX=19;
const uint64_t PUSH=20;
const uint64_t POP=21;
const uint64_t MOV=22;
const uint64_t SET=23;
const uint64_t NOP=24;
const uint64_t CBZ=25;
const uint64_t CBNZ=26;
const uint64_t B=27;
const uint64_t ADR=28;
const uint64_t WFI=29;

// REGISTERS
const uint64_t NREGS=16;
const uint64_t R0=0;
const uint64_t R1=1;
const uint64_t R2=2;
const uint64_t R3=3;
const uint64_t R4=4;
const uint64_t R5=5;
const uint64_t R6=6;
const uint64_t R7=7;
const uint64_t R8=8;
const uint64_t R9=9;
const uint64_t SB=9;
const uint64_t R10=10;
const uint64_t SL=10;
const uint64_t R11=11;
const uint64_t FP=11;
const uint64_t R12=12;
const uint64_t SP=13;
const uint64_t LR=14;
const uint64_t PC=15;

// Memory Map,for the actual memory
// Lower 8MB is heap
// next 4M is stack
// next 2MB is code
const uint64_t IVT=0x0;
const uint64_t HEAP=IVT+IVTsz;
const uint64_t STACK=HEAP+heapSz;
const uint64_t CODE=STACK+stackSz;
const uint64_t MEMTOP=CODE+codeSz;
const uint64_t TIMER=MEMTOP;
const uint64_t KBD=TIMER+timerStateSz;
const uint64_t NIC=KBD+kbdStateSz;
const uint64_t SSD=NIC+nicStateSz;
const uint64_t GPU=SSD+ssdStateSz;
const uint64_t STATETOP=GPU+gpuStateSz;

#endif
