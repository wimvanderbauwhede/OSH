#!/usr/bin/env perl
use v5.24;
use warnings;
use strict;
use Data::Dumper;
@ARGV or die "$0 <python file>";
my $pyf=$ARGV[0];
my $base_name = $pyf;
$base_name =~s/\.py$//;
my $ccf=$base_name.'.cc';
my $cchf=$base_name.'.h';

=pod
What we need to do is:
1. Syntax: 
- on every non-blank line 
    - without ':', add a ';'
    - with a ':' add a '{'
        - def: add types, inventory
        - elif: ad '}'
- on every line where the indent is shorter than the previous non-blank line, add a '}'        

=cut
open my $PY, '<', $pyf or die $!;
my @py_lines=();
while(my $line=<$PY>) {
    chomp $line;
    push @py_lines, $line;
}
close $PY;

# ---------------- -------- -------- -------- -------- -------- -------- -------- 
# 0. Cleaning up a bit
for my $line (@py_lines) {
    $line=~s/\s+$//;
    $line=~s/^\s+#/#/;
    $line=~s/np.uint64//g;
    $line!~/^\s*(def|print)/ && $line=~s/(\W\s*)\((\w+)\)/$1$2/g;
    $line=~s/\s*=\s*/=/g;
    $line=~s/\s*,\s*/,/g;
    #    $line!~/return\s+\(/ && $line=~s/\s*\(\s*/\(/g;
    $line=~s/\(\s*/\(/g;
    $line=~s/\s*\)\s*/\)/g;
    #    $line=~s/None/(uint64_t)0/g;
    # Not nice but easy
    $line=~s/\s+\#.+$//;
}
#die Dumper(@py_lines);
# ---------------- -------- -------- -------- -------- -------- -------- -------- 
# 1. Analysis. Collect var decls etc
my %func_args=();
my %var_decls=();
my %parent_func=();

# Two passes to resolve all variables
my $current_func='GLOBAL';
my $indent='';
my $prev_indent='';

my @indent_stack=($indent);
my @funcs_stack=('GLOBAL');
my %def_indents=();
my $prev_line='';

for my $iter (1..2) {
$current_func='GLOBAL';
$indent='';
$prev_indent='';

@indent_stack=($indent);
@funcs_stack=('GLOBAL');

%def_indents=();
$prev_line='';

for my $line (@py_lines) {
    #- on every non-blank line 
    if ($line!~/^\s*$/
            and $line!~/^\s*\#/
    ) {
        #        say "LINE: $line FUNC: $current_func";
        if ($line=~/^(\s+)\S/) {
            $indent=$1;
        } else {
            $indent='';
        }

    say "LINE: $line";
    if (length($prev_indent) > length($indent)) {
        while (length($indent_stack[-1]) > length($indent)) {
            my $popped_indent = pop @indent_stack;
            say "POP INDENT";
            my $top_indent = $indent_stack[-1];
            #            say "<$popped_indent>";
            if (exists $def_indents{$top_indent} ) {
                my $top_func = $def_indents{$top_indent};
                #my $parent_func_name  = $parent_func{$top_func};
                my $popped_func = pop @funcs_stack;
                my $parent_func_name = $funcs_stack[-1] ;
                say "POP: $popped_func >> $line => PARENT: ".$parent_func_name ;
                $current_func=$parent_func_name ;

            }
        }
    }


        if (length($indent)>length($prev_indent) and $prev_line=~/:\s*$/ ) {
            say "PUSH INDENT";
            push @indent_stack,$indent;
        }        
     
    # def
    if ($line=~/^\s*def\s+(\w+)\s*\((.+)\)\s*:/) { 
        my $func_name=$1;    
        #$parent_func{$func_name}=$current_func;
        say "PARENT: ".$funcs_stack[-1]."=> PUSH: $func_name ".Dumper(@indent_stack);
        #        $current_func";
        $current_func=$func_name;
        push @funcs_stack, $current_func;

         
        $def_indents{$indent}=$current_func;
        # FIXME: should be a stack, see below!
        #            say "in function $current_func";
        # I use 'def' as the "name of the return argument"
        $func_args{$current_func}={'def' => 'auto'};
        my $args_str=$2;
        my @args=split(/\s*,\s*/,$args_str);
        for my $arg (@args) {
            $func_args{$current_func}{$arg}=$arg.'_t';#'uint64_t';
        }
    }
    # scalar assignment
    if ($line=~/^\s*(\w+)\s*[\-\+]?=\s*(.+)\s*$/) {
        my $var=$1;
        my $rhs=$2;
        if (not exists $var_decls{$current_func}) {
            $var_decls{$current_func}={};
        }
        if (
            not exists  $func_args{$var}
        ) {
            $var_decls{$current_func}{$var}='uint64_t'
        }
        # check if $rhs is a tuple
        if ($rhs=~/^\(([\w\s*,]+)\)$/) {
            # that means it is a tuple
            my $tupvars=$1;
            my @vars=split(/\s*,\s*/,$tupvars);
            # We set the tuple type to scalar uint64_t by default
            my $type='std::tuple<'.join(',',map { 
                    exists $var_decls{$current_func}{$_} ? $var_decls{$current_func}{$_}
                    : exists  $func_args{$current_func}{$_} ? $func_args{$current_func}{$_} : 'uint64_t' 
                } @vars).'>';
            $var_decls{$current_func}{$var}=$type;
        }
        # check if $rhs is an array
        if ($rhs=~/^\[(.+)\]$/ or $rhs=~/^np.\w+/) { 
            if (exists $var_decls{$current_func}{$var}) {
                $func_args{$current_func}{$var}='std::vector<uint64_t>&';
            } else {
                $var_decls{$current_func}{$var}='std::vector<uint64_t>';
            }
        }

    }
    # tuple assignment
    if ($line=~/^\s*\((.+)\)\s*=\s*(.+)\s*$/) {
        my $vartup=$1;
        my $rhs=$2;

        my @vars=split(/\s*,\s*/,$vartup);

        if (not exists $var_decls{$current_func}) {
            $var_decls{$current_func}={};
        }
        for my $var (@vars) {
            if (
                 not exists  $func_args{$current_func}{$var}
            ) {
                # by default we assume scalars are uint64_t
                if (not exists $var_decls{$current_func}{$var}) {
                    $var_decls{$current_func}{$var}=$var.'_t';
                }
            }
        }
        # check if $rhs is a scalar
        if ($rhs=~/^(\w+)$/) {
            # that means it is a tuple
            #say $line;
            #say $rhs,@vars;
            my $type='std::tuple<'.join(',',map { 
                    exists $var_decls{$current_func}{$_} ? $var_decls{$current_func}{$_}
                    : exists  $func_args{$current_func}{$_} ? $func_args{$current_func}{$_} : 'uint64_t' 
                } @vars).'>';
            $var_decls{$current_func}{$rhs}=$type;
        }

    }
    # array assignment
    if ($line=~/^\s*(\w+)\[/ or $line=~/^\s*(\w+)\s*=\s*\[/ ) {
        my $array_var=$1;
        if (exists  $func_args{$current_func}{$array_var}) {
            $func_args{$current_func}{$array_var}='std::vector<uint64_t>&';
        }
        #        elsif (exists  $var_decls{$current_func}{$array_var}) {
            $var_decls{$current_func}{$array_var}='std::vector<uint64_t>';
            #}
    }
    # array access
    if ($line=~/(\w+)\[/) {
        my $array_var=$1;
        if (exists  $func_args{$current_func}{$array_var}) {
            $func_args{$current_func}{$array_var}='std::vector<uint64_t>&';
        }
        elsif (exists  $var_decls{$current_func}{$array_var}) {
            $var_decls{$current_func}{$array_var}='std::vector<uint64_t>';
        }

    }
    if ($line=~/^\s*for\s+(\w+)\s+in\s+(\w+)\s*:/) {
        my $iter=$1;
        my $array_var=$2;
        $var_decls{$current_func}{$iter}='uint64_t';
        my $type=$var_decls{$current_func}{$array_var} ;
        $var_decls{$current_func}{$array_var}='std::vector<uint64_t>';
    }
    # check if return statement is a tuple
    if ( $line=~/^\s+return\s*\((.+)\)/) {    
        my $tupvars=$1;
        #retval        say "return tuple $tupvars";
        my @vars=split(/\s*,\s*/,$tupvars);
        my $type='std::tuple<'.join(',',map { 
                exists $var_decls{$current_func}{$_} ? $var_decls{$current_func}{$_}
                : exists  $func_args{$current_func}{$_} ? $func_args{$current_func}{$_} : $_.'_t' 
            } @vars).'>';
        $func_args{$current_func}{'def'}=$type;
    } elsif ($line=~/^\s+return\s+\[/) {
        $func_args{$current_func}{'def'}='std::vector<uint64_t>';
    } elsif ($line=~/^\s+return\s+(\w+)/) {
        my $retval=$1;
        my $type = 'auto';
        if (exists $func_args{$current_func}{$retval}) {
            $func_args{$current_func}{'def'}=$func_args{$current_func}{$retval};
        }
        elsif (exists  $var_decls{$current_func}{$retval}) {
            $func_args{$current_func}{'def'}=$var_decls{$current_func}{$retval};
        }
    }
    $prev_indent=$indent;
    $prev_line=$line;
}
}


} # for 1..2
# ---------------- -------- -------- -------- -------- -------- -------- -------- 

say "Var decls";
say Dumper(%var_decls);
say "Function args";
say Dumper(%func_args);
#die;
# ---------------- -------- -------- -------- -------- -------- -------- -------- 
# Emit C++
my $has_functions=0;
my @cpp_lines=( "#include <tuple>");
my @cpp_header_lines=("#include <vector>","#include <tuple>");
$current_func='GLOBAL';
my $indent_step=0;
@indent_stack=('');
my $skip=0;
my $ndecls=0;
for my $line (@py_lines) {
    #- on every non-blank line 
   if ($line!~/^\s*$/
            and $line!~/^\s*\#/
    ) {
        #        say "ORIG LINE: $line";
        if ($line=~/^(\s+)\S/) {
            $indent=$1;
        } else {
            $indent='';
        }
        if (length($indent)>length($prev_indent) and $prev_line=~/\{$/ ) {
            $indent_step++;
            push @indent_stack,$indent;
            say "> PREV_LINE: <$prev_line><$prev_indent>";
            say ">      LINE: <$line><$indent>";
        #       say "LEN: ".length($indent);
        #        say "PREV_LEN: ".length($prev_indent);
        }
        if ($line=~/from\s+(\w+)\s+import/ or
            $line=~/import\s+(\w+)/) {
            my $module=$1;
            next if $module eq 'numpy';
            $line = '#include "'.$module.'.h"';
            push @cpp_lines, $line;
            $prev_line=$line;
            $prev_indent=$indent;
            $skip=1;
            #            next;
            }
        if ($line=~/:\s*$/) { 
            $line=~s/:\s*$/ {/;
            if ($line=~/^\s*def\s+(\w+)\((.+)\)/) { 
                $current_func=$1;     
                my $argtup=$2;
                #say "DEF LINE: $line";
                $has_functions++;
                my @args=split(/,/,$argtup);           
                $line = $indent.$func_args{$current_func}{'def'}.' '.$current_func.'(';
                my @decls=();
                for my $arg ( @args ) {
                    my $type = $func_args{$current_func}{$arg};
                    my $decl = "$type $arg";
                    push @decls,$decl;
                }
                $line.=join(',',@decls).')';
                push @cpp_header_lines, $line.';';
                $line.='{';
                push @cpp_lines, $line; 
                $ndecls=1;
                for my $var (sort keys %{$var_decls{$current_func}}) {
                    if (not exists $func_args{$current_func}{$var}) {
                    ++$ndecls;
                    my $type = $var_decls{$current_func}{$var};
                    my $var_decl_line=$indent.'    '.$type.' '.$var.';';
                    push @cpp_lines, $var_decl_line;
                }
                }
                push @cpp_lines, $indent.'    uint64_t None=0;';                
                $skip=1;
                #                next;

            } elsif ($line=~/^\s*(if|while)\s/) {
                $line=~s/if /if (/;
                $line=~s/while /while (/;
                $line=~s/\{$/) {/; 
            } elsif ($line=~/^\s*for\s(\w+)/) {
                my $var = $1;
                $line=~s/for /for (/;
                $line=~s/\{$/) {/; 
                $line=~s/\ in\ / : /;
                $line=~s/$var/auto $var/;
            } elsif ($line=~/^\s*elif\s/) { 
                $line=~s/elif / else if (/;
                $line=~s/\{$/) {/; 
            } elsif ($line=~/^\s*else\s*\{/) { 
                $line=~s/else / else /;
            }
        } else {
            # not a block line; but this is WEAK!
            $line=~s/\s*$/;/;
        }    
                
        if ( $line=~/^\s*\([\w,]+\)\s*=/) {
            $line=~s/\(/std::tie(/;
        }
        if ( $line=~/=\s*\([\w,]+\)\s*;$/) {
            $line=~s/=/= std::make_tuple/;
        }
        elsif ( $line=~/^\s*return\s*\(.+?\)/) {
            $line=~s/return\s+/return std::make_tuple/;        
        }
        # for constants
        elsif ($line=~/^\s*[A-Z][A-Z_0-9]+=/ ) {
            $line = 'const uint64_t '.$line;
        }
        elsif ($current_func eq 'GLOBAL' and $line=~/^\s*\w+=(0x)?\d+/) {
            $line = 'uint64_t '.$line;
        }
        # for arrays of arrays/tuples
        elsif ($line=~/^\s*(\w+)=\[;$/) {
            my $vec=$1;
            $line = $indent.'std::vector< std::vector<uint64_t>> '.$vec.'={';
        }
        elsif ($line=~/^\s*\[[\w,]+\],?;$/) {
            $line=~s/\[/{_,/;
            $line=~s/\]/}/;
            $line=~s/;//;
        }
        elsif ($line=~/^\s*\(\w+,\[[\w,]+\]\)/){#,?;$/) {
            $line=~s/\(/{/;
            $line=~s/\)/}/;
            $line=~s/\[//;
            $line=~s/\]//;
            $line=~s/\'//g;
            $line=~s/;//;
        }
        elsif ($line=~/^\s+\];$/) {
            $line=$indent.'};';
        }

        if (length($prev_indent) > length($indent)) {
        say "<PREV LINE: $prev_line";
        say "<     LINE: $line";
        #        say Dumper(@indent_stack,$indent,$prev_indent,$skip);
            $indent_step--;
            while (length($indent_stack[-1]) > length($indent)) {
                my $popped_indent=pop @indent_stack;
                my $top_stack_indent = scalar @indent_stack > 0 ? $indent_stack[-1] : '';
                my $closing_brace = $top_stack_indent.'}//'.length($prev_indent).' > '.length($indent); 
                
                if ($line=~/^\s+(else|elif)/) {
                    my $w=$1;
                    $line=~s/$w/} $w/;
                } else {
                    if ($skip) {
                        splice @cpp_lines, scalar( @cpp_lines ) -$ndecls-1, 0, ($closing_brace,'');
                        #   @cpp_lines[0 .. scalar @cpp_lines -$ndecls-2],
                        #   $closing_brace,
                        #   @cpp_lines[-$ndecls-1 .. -1]
                        #);
                        #die Dumper(@cpp_lines);
                    } else {
                        push @cpp_lines, $closing_brace;
                    }
                }
            }
        }        
        $prev_indent=$indent;
        $prev_line=$line;
    }
    if ($line=~/^\s*\#/) {
        $line=~s/^\s*\#/\/\//;
    }
 
    push @cpp_lines, $line unless $skip;
    $skip=0;
}

if(@indent_stack) {
     push @cpp_lines, '}' x (scalar( @indent_stack)-1);
}

#die Dumper(@cpp_lines);
if ($has_functions) {
    open my $CC, '>', $ccf;
    for my $line (@cpp_lines) {
        say $CC $line;
    }
    close $CC;
    #    say "HEADERS";

    open my $CCH, '>', $cchf;
    for my $line (@cpp_header_lines) {
        say $CCH $line;
    }
    close $CCH;
} else {
    # No functions, use cpp_lines as content of header file
    open my $CCH, '>', $cchf;
    for my $line (@cpp_lines) {
        say $CCH $line;
    }
    close $CCH;
}
