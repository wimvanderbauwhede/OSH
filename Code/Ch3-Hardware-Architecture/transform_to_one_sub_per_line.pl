#!/usr/bin/env perl
use v5.20;
use strict;
use warnings;

=pod
This script transforms C subroutines so that every line becomes a separate subroutine. 
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


=cut

die "$0 tasks-source.c" unless @ARGV;
open my $IN, '<', $ARGV[0] or die $!;


my %globals=();
my %tasks=();
my @lines = ();
my $current_task='NO_TASK';

# Read the input file, splitting into lines per subroutine
for my $line (<$IN>) {
    #    say "LINE: $line";
    # Start of sub
    $line=~/void\s+(\w+)\s*\(\s*void\s*\)\s*\{/ && do {
        $current_task=$1;
        $tasks{$current_task}=[];
        next;
    };
    # End of sub
    $line=~/^\s*\}\s*$/ && do { 
        $current_task='NO_TASK';
        next;
    };
    #    say "TASK: $current_task";
    # Type declarations are detected and turned into global declarations
    $current_task ne 'NO_TASK' && $line=~/(?:int|float|char|long|double)\s+/ && do {
            my $var = $line;            
            $var=~/^.+\s+(\w+\[?\w*\]?)\s*;/ && do {
                $var =$1;
                #                say "VAR: $var";
                if (exists $globals{$var}) {
                    die "Variable $var in task '$current_task' is already defined in task '".$globals{$var}[1]."'. Please change the variable name so that it is globally unique.";
                } else {
                    my $glob_line = $line;
                    chomp $glob_line;
                    $glob_line=~s/^\s*static\s+//;
                    $globals{$var}= ['    static '.$glob_line, $current_task];
                }
            };
            next;
    };
    push @{ $tasks{$current_task} }, $line unless $line=~/^\s*$/; 
}
close $IN;

# Generated lines
my @gen_lines=@{ $tasks{'NO_TASK'} };
delete $tasks{'NO_TASK'};

# Global type declarations
for my $glob_var (sort keys %globals) {
    push @gen_lines, $globals{$glob_var}[0];
}

# Create global type declarations and function signatures
for my $task (sort keys %tasks) { # This should be a subroutine I think

    my @lines = @{ $tasks{$task} };
    my $NINSTRS = scalar @lines;    
    my $NINSTRS_VAR = 'NINSTRS_'.uc($task);
     
    push @gen_lines,'// Declarations of per-line functions for '.$task;
    push @gen_lines,'#define '.$NINSTRS_VAR.' '.$NINSTRS;
    push @gen_lines,'void (*'.$task.'_instrs['.$NINSTRS_VAR.']) (void);';


    for my $id ( 0 .. $NINSTRS-1) {
        push @gen_lines,'void '.$task.'_line'.$id.'(void);'
    }
    push @gen_lines,'';
}


# Create main sub start
    push @gen_lines,'static int pc;';
    push @gen_lines,'static int system_timer;';
    push @gen_lines,'static int blocking_timer;';
    push @gen_lines,'';
    push @gen_lines,'int main(){ //}';
    push @gen_lines,'    system_timer=0;';
    push @gen_lines,'    blocking_timer=0;';

# Create task code
for my $task (sort keys %tasks) { 
    generate_task_code($task);    
}

# Create main sub end

    push @gen_lines,'    return 0;';
    push @gen_lines,'}';
    push @gen_lines,'';

    #    say '// Generated code for per-line functions for '.$task;
    my $is_blank=0;
    for my $gen_line (@gen_lines) {
        say $gen_line;
        if ($gen_line=~/^\s*$/) {
            $is_blank=1;
        } else {
            $is_blank=0;
        }
    }

# Generate per-line subroutine definitions

for my $task (sort keys %tasks) { # This should be a subroutine I think
    my @func_lines=();
    my @lines = @{ $tasks{$task} };

#- create a map of labels to line numbers
    my %labels_to_line_numbers =();
    
    my $line_number=0;
    for my $line (@lines) {
        chomp $line; 
        if ($line=~/^(\w+):/ ) {
            my $label=$1;
            $labels_to_line_numbers{ $label } = $line_number;
        }
        $line_number++;
    }
    $line_number=0;
    for my $line (@lines) {
        #say $line;
        chomp $line; 
        if ($line=~/^(\w+):/ ) {
            $line=~s/^(\w+)://;
            $line.= 'pc++;';
        } 
        if ( $line=~/goto\s+(\w+)/ ) {
            my $label=$1;
            my $id = $labels_to_line_numbers{$label};
            if ($line=~/^\s*if\s*\(.+?\)\s+goto/) {
                $line=~s/goto.+$//;
                $line.= '{ pc ='.$id.'; } else { pc++;}'
            } else {
                $line="    pc = $id;";
            }
        } else {
            $line.= 'pc++;';
        }
        my $flines = make_func($task,$line,$line_number);
        push @func_lines,$flines;
        $line_number++;
    }
    say '// Generated per-line function code for '.$task;
    my $is_blank=0;
    for my $gen_line (@func_lines) {
        say $gen_line;
        if ($gen_line=~/^\s*$/) {
            $is_blank=1;
        } else {
            $is_blank=0;
        }
    }
}


sub generate_task_code { (my $task) = @_;

    my @lines = @{ $tasks{$task} };
    my $NINSTRS = scalar @lines;
    my $NINSTRS_VAR = 'NINSTRS_'.uc($task);    
#no blocks allowed so all control is via goto
#lines should only have conditional statements, no blocks
#if we allow local vars they must be mapped to global vars

#- create a map of labels to line numbers
    my @func_lines=();
  
    push @gen_lines,'';
    for my $id ( 0 .. $NINSTRS-1) {
        push @gen_lines,'    '.$task.'_instrs['.$id.']='.$task.'_line'.$id.';';
    }


    push @gen_lines,'    pc=0;';
    push @gen_lines,'CALL_'.uc($task).': '.$task.'_instrs[pc]();       ';
    push @gen_lines,'      if (pc=='.$NINSTRS_VAR.') goto END_'.uc($task).';';
    push @gen_lines,'      goto CALL_'.uc($task).';';
    push @gen_lines,'END_'.uc($task).':';
    push @gen_lines,'      // return 0;';

    #    say '// Generated code for per-line functions';
    #my $is_blank=0;
    #for my $gen_line (@gen_lines) {
    #    say $gen_line;
    #    if ($gen_line=~/^\s*$/) {
    #        $is_blank=1;
    #    } else {
    #        $is_blank=0;
    #    }
    #}
}

sub make_func { (my $task,my $line, my $id)=@_;
    my $fline =
    'void '.$task.'_line'.$id.'(void) {';
    $fline.=$line;
    $fline.='}';
    return $fline;
}

