The script `transform_to_one_sub_per_line.pl` transforms C subroutines so that every line becomes a separate subroutine. 

Assumptions on the input:

- The input file contains global variable declarations and subroutines.
- Inside each subroutine there are no blocks, only single-line statements.
- Subroutine-scoped variables are allowed but variable names must be unique across all subroutines.

If we want to simulate blocking behaviour, we at least need to count for how long something blocks. The easies way is with a global 'blocking_timer' which will decrement. 
So I think we do not only need a pc but also a system clock or timer which will increment monotonically.
Every call will increment this system_timer. 

So then blocking IO is a matter of setting a blocking timer to some time in the future, and when that time is reached we can unblock the task. 

    void blocking_io (int blocking_time) {
        blocking_timer = system_timer + blocking_time);
    }

Another obvious addition is save_context() and restore_context()
I guess this requires a task ID, so we need some kind of task_id counter and a way to link the task to its id, probably a macro.

Then we can do save_context(task_id) and restore_context(task_id)

We need a convention for registers, so let's trivially say 

    r0-r12, sp, lr, pc
    #define NREGS  16
    #define STACK_SZ 16*1023
    #define CONTEXT_SZ NREGS+STACK_SZ
    We create a global array int context[NTASKS][CONTEXT_SZ]

What this will do is simply put these regs in the lower part of the contect, and read them back on restore.

    save_context(int task_id) {
        context[task_id][0] = r0;
        context[task_id][1] = r1;
        context[task_id][2] = r2;
        context[task_id][3] = r3;
        context[task_id][4] = r4;
        context[task_id][5] = r5;
        context[task_id][6] = r6;
        context[task_id][7] = r7;
        context[task_id][8] = r8;
        context[task_id][9] = r9;
        context[task_id][10] = r10;
        context[task_id][11] = r11;
        context[task_id][12] = r12;
        context[task_id][13] = sp;
        context[task_id][14] = lr;
        context[task_id][15] = pc;
    #ifdef HAS_STACK
    // if we have a single stack then we need to copy every element
        for (int i =0; i<STACK_SZ; ++i) {
             context[task_id][i+!5]=stack[i];
        }
    #endif
    }

    void restore_context(int task_id) {

    r0 = context[task_id][0];
    r1 = context[task_id][1];
    r2 = context[task_id][2];
    r3 = context[task_id][3];
    r4 = context[task_id][4];
    r5 = context[task_id][5];
    r6 = context[task_id][6];
    r7 = context[task_id][7];
    r8 = context[task_id][8];
    r9 = context[task_id][9];
    r10 = context[task_id][10];
    r11 = context[task_id][11];
    r12 = context[task_id][12];
    sp = context[task_id][13];
    lr = context[task_id][14];
    pc = context[task_id][15];


    #ifdef HAS_STACK
    // if we have a single stack then we need to copy every element
    for (int i =0; i<STACK_SZ; ++i) {
        stack[i]=context[task_id][i+!5];
    }
    #endif
    }

