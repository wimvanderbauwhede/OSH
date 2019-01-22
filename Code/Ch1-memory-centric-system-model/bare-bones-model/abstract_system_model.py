#!/usr/bin/env python
"""
A model for a processor-based system. 
"""

from abstract_system_constants import *
from abstract_system_peripherals import *
from abstract_system_processor import processorAction
from abstract_system_code import * 

import sys, getopt

def main():

# Default settings
    PRINT_CYCLES=False

    MAX_NCYCLES= 1

# Command line flags and arguments
    try:
        opts, args = getopt.getopt(sys.argv[1:],"hsN:")
    except getopt.GetoptError:
        print( 'abstract_system_model.py. [-hsN:]')
        sys.exit(2)

    for opt, arg in opts:
      if opt == '-h':
         print( 'abstract_system_model.py [-hs] -N ncycles'+"\n", 
            '-N : number of cycles to run'+"\n",
            '-s: show cycle count'
            )
         sys.exit()
      elif opt in ('-s'):
          PRINT_CYCLES=True
      elif opt in ('-N'):
          MAX_NCYCLES=int(arg)

# Initialise the state

    systemState=[0]*systemStateSz # without numpy

    timerState=[0]*timerStateSz
    timerState[0]=MAX_NCYCLES+1
    timerState[1]=MAX_NCYCLES+1
    kbdState=[0]*kbdStateSz
    nicState=[0]*nicStateSz
    ssdState=[0]*ssdStateSz
    gpuState=[0]*gpuStateSz
    ramState=[0]*ramStateSz

# Initialise the registers
    registers=[0]*16 #[0]*16
# Set the program counter to the start of the code region    
    registers[PC]=CODE
    registers[R1]=6
    registers[R2]=7
    registers[R3]=0

    ramState = load_code(ramState)

    systemState = ramState+timerState+kbdState+nicState+ssdState+gpuState

    for ncycles in range(0,MAX_NCYCLES):
            if PRINT_CYCLES:
                print(ncycles)
            kbdState=kbdAction(kbdState)
            nicState=nicAction(nicState)
            ssdState=ssdAction(ssdState)
            gpuState=gpuAction(gpuState)
            timerState=timerAction(timerState)
            ramState=systemState[0:MEMTOP]

                
            systemState = ramState+timerState+kbdState+nicState+ssdState+gpuState # without numpy
            (systemState,registers) = processorAction(systemState,registers)

    print(registers[R3])
############ ############ ############ ############ ############ ############ ############ ############ 

main()
