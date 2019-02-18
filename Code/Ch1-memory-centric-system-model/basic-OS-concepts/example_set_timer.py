from abstract_system_constants import *

# Instructions to set the timer to periodic with 100-ticks interval
set_timer=[
	[SET,R1,100],
	[SET,R2,100], # start periodic timer
	[STR,R1,TIMER],
	[STR,R2,TIMER+1]
]
