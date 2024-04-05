### Processor interrupt handling
from abstract_system_constants import *
def checkIrqs(registers,ivt,irqs):
    idx=0
    for irq in irqs:
        if irq :
            print("IRQ ",idx," raised!")
            # Clear the WFI status 
            registers[SCR] = 0
            # Save the program counter in the link register
            registers[LR] = registers[PC]
            # Set program counter to ISR start address
            registers[PC]=ivt[idx]
            irqs[idx]=False
            break
        idx+=1        
    return (registers,irqs)

