from abstract_system_constants import *

### Peripheral actions

def kbdAction(kbdState):
    kbdIrq=False
    return (kbdState,kbdIrq)        

def nicAction(nicState):
    nicIrq=False
    return (nicState,nicIrq)  

def ssdAction(ssdState):
    ssdIrq=False
    return (ssdState,ssdIrq)  

def gpuAction(gpuState):
    gpuIrq=False
    return (gpuState,gpuIrq)  

def timerAction(timerState):
    # print('timerAction',timerState)
    if timerState[0]>0:
        timerState[0]-=1
    timerIrq=False
    if timerState[0]==0:
        # print('timerAction: timerIrq SET')
        timerIrq=True
        timerState[0]=timerState[1]
    return (timerState,timerIrq)

def dmaAction(systemState):
    dmaIrq=0
    # DMA is the start of the address space
    # DCR values: 1 = do transfer, 0 = idle
    if  systemState[DMA+DCR]!=0:
        if systemState[DMA+DCO]!=0:
            ctr = systemState[DMA+DCO]
            to_addr = systemState[DMA+DDR]+ctr
            from_addr = systemState[DMA+DSR]+ctr             
            systemState[to_addr] = systemState[from_addr]
            systemState[DMA+DCO]=-1
        systemState[DMA+DCR]=0
        dmaIrq=1
    return (systemState,dmaIrq)
