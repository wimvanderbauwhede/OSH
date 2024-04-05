#!/usr/bin/env python
"""
A model for a processor-based system. 
The main difference from the code in the book is that it uses NumPy arrays for performance.
The model for loading high-level code is different as well.
"""


from abstract_system_constants import *
from abstract_system_peripherals import *
from abstract_system_processor import processorAction
from abstract_system_code import * #load_code

import numpy as np
import sys, getopt

def main():
    global PERIPHERALS, HL
    global hlCode

# Default settings
    PERIPHERALS=False
    HL=False
    PRINT_CYCLES=False

    MAX_NCYCLES= 512
    if HL:
        MAX_NCYCLES= 1

# Command line flags and arguments
    try:
        opts, args = getopt.getopt(sys.argv[1:],"hPHsN:")
    except getopt.GetoptError:
        print( 'abstract_system_model.py. [-hPHN:s]')
        sys.exit(2)

    for opt, arg in opts:
      if opt == '-h':
         print( 'abstract_system_model.py [-hPHc] -N ncycles'+"\n", 
            '-P : with peripherals'+"\n", 
            '-H : high-level (not cycle-accurate)'+"\n",
            '-N : number of cycles to run'+"\n",
            '-s: show cycle count'
            )
         sys.exit()
      elif opt in ("-P"):
         PERIPHERALS=True
      elif opt in ("-H"):
         HL = True
      elif opt in ('-s'):
          PRINT_CYCLES=True
      elif opt in ('-N'):
          MAX_NCYCLES=int(arg)

# If we use high-level code we need to store it in a separate array 
# because the NumPy arrays are declared for uint64
    # hlCode=[]
    # if HL:
        # Fill the code array with no-op functions
    hlCode=[lambda s,r: (s,r) ]*hlCodeSz

    # Configuration of the model
    cfg=(PERIPHERALS,HL,hlCode)

# Initialise the state

    #systemState= [0]*systemStateSz # without numpy
    systemState=np.zeros( systemStateSz,dtype=np.uint64 ) 

    timerState=np.zeros( timerStateSz,dtype=np.uint64 )
    timerState[0]=MAX_NCYCLES+1
    timerState[1]=MAX_NCYCLES+1
    kbdState=np.zeros( kbdStateSz,dtype=np.uint64 )
    nicState=np.zeros( nicStateSz,dtype=np.uint64 )
    ssdState=np.zeros( ssdStateSz,dtype=np.uint64 )
    gpuState=np.zeros( gpuStateSz,dtype=np.uint64 )
    dmaState=np.zeros( dmaStateSz,dtype=np.uint64 )
    ramState=np.zeros( ramStateSz,dtype=np.uint64 )

# Initialise the registers
    registers=np.zeros( NREGS,dtype=np.uint64 ) #[0]*NREGS
# Set the program counter to the start of the code region    
    registers[PC]=CODE

# Initialise the intterupt vector table    
    ramState[IVT:IVT+IVTsz]=[CODE+18]*IVTsz # TODO: use IRS addresses

    (ramState,cfg) = load_code(ramState,cfg)

    #systemState = ramState+timerState+kbdState+nicState+ssdState+gpuState
    systemState = np.concatenate((ramState,timerState,kbdState,nicState,ssdState,gpuState,dmaState))
    for ncycles in range(0,MAX_NCYCLES):
            if PRINT_CYCLES:
                print(ncycles)
            if PERIPHERALS:
                (kbdState,kbdIrq)=kbdAction(kbdState)
                (nicState,nicIrq)=nicAction(nicState)
                (ssdState,ssdIrq)=ssdAction(ssdState)
                (gpuState,gpuIrq)=gpuAction(gpuState)
                (timerState,timerIrq)=timerAction(timerState)
                (systemState,dmaIrq) = dmaAction(systemState)
                
                ramState=systemState[0:MEMTOP]

                irqs=[kbdIrq,nicIrq,ssdIrq,gpuIrq,timerIrq,dmaIrq]
                
#                systemState = ramState+timerState+kbdState+nicState+ssdState+gpuState # without numpy
                systemState = np.concatenate((ramState,timerState,kbdState,nicState,ssdState,gpuState,dmaState))
                timerState = systemState[TIMER:TIMER+timerStateSz-1]
                # TODO: other peripherals
            else:
                irqs=[]
#            print( systemState[CODE:CODE+20] )
            (systemState,irqs,registers) = processorAction(systemState,irqs,registers,cfg)

    print(systemState[IVT+IVTsz:FIB_MAX])

############ ############ ############ ############ ############ ############ ############ ############ 

main()
