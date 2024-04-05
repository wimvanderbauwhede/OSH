from abstract_system_constants import *

NREGS_CONTEXT=16
def time_slice(systemState,registers ):
# Push registers onto the stack
    for r in range(0,NREGS_CONTEXT):
      systemState[registers[MSP]]=registers[r]
      registers[MSP]+=1
# Get next task
    pid1 =systemState[PID] # 0 or 1
    pid2 = 1-pid1 # 2 tasks only
    systemState[PID]=pid2
    tcb1= TCB_OFFSET+pid1*TCB_SZ
    tcb2= TCB_OFFSET+pid2*TCB_SZ
# Pop registers from stack and store to tcb1
# We use r0 to show that in actual code we'd need to read into a temporary register
    for r in range(0,NREGS_CONTEXT):
        r0=systemState[registers[MSP]]
        systemState[tcb1+r]=r0
        registers[MSP]-=1
# Push registers for Task 2 from tcb2 onto stack
    for r in range(0,NREGS_CONTEXT):
        r0=systemState[tcb2+r]
        systemState[registers[MSP]]=r0
        registers[MSP]+=1
# Pop registers for Task 2 from stack
    for r in range(0,NREGS_CONTEXT):
        registers[r]=systemState[registers[MSP]]
        registers[MSP]-=1
    return ( systemState,registers )
