[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/4mSnmR_O)
# INF2C-CS Coursework 2
![BuildAndTest](../../workflows/BuildAndTest/badge.svg?branch=main) ![Points badge](../../blob/badges/.github/badges/points.svg)
## Important Dates

- Coursework 2 Release Date: November 8, 2024
- Coursework 2 Submission Deadline: November 29, 2024 (4pm)

## Overview

This exercise has three aims: (a) It will provide you with further
opportunity to develop your C programming skills within a slightly
larger project, (b) it deepens your understanding of MIPS assembly, and
(c) it will introduce you to concepts of machine instruction decoding
and execution. This assignment asks you to write an interpretative MIPS
instruction set simulator in C, i.e. a piece of software that behaves
like a real MIPS processor and which can execute MIPS machine code
instruction-by-instruction. In principle, this is your own version of
the simulator core in the MARS IDE that you have been using earlier on
in the course.

This is the second of two coursework assignments for the INF2C Computer
Systems course. It is worth **50% of the coursework mark** for INF2C-CS
and **20% of the overall course mark**.

Instruction set simulators are commonly used tools, supporting designers
of new processors, but also software developers, e.g. those developing
software for processors different to those in their desktop PCs (think
of ARM based mobile phones and the Android emulators for PCs).

The main focus of this exercise is to implement an instruction set
simulator for a subset of the MIPS machine language in C. You should be
able to run the MIPS programs you have developed in coursework 1 on your
own simulator!

## Learning Outcomes

This coursework is closely aligned with the learning outcomes of the
INF2C-CS course as specified in its course descriptor
(<http://www.drps.ed.ac.uk/24-25/dpt/cxinfr08027.htm>).

*On completion of this course, the student will be able to:*

1.  *Describe the trade-offs in different binary representation
    systems.*

2.  ***Explain the principles of: instruction set architecture**,
    digital logic design, cache hierarchy, virtual memory, I/O devices,
    exceptions and processor management.*

3.  ***Demonstrate an understanding of how a high-level programming
    language (C) maps to the assembly code by converting a simple C
    program to MIPS assembly.***

4.  *Sketch the design of a simple single- and multi-cycle processor and
    **explain how it operates** by combining the knowledge of the logic
    design basics with that of the **MIPS instruction set
    architecture**.*

In this coursework you will develop an (incomplete) MIPS instruction set
simulator using the C programming language. This will give you an
opportunity to exercise your C programming skills. At the same time,
this coursework will help you deepen your understanding of the MIPS
assembly and the MIPS instruction set architecture by implementing a key
instruction decoding and execution stages, demonstrating the
instruction-by-instruction operation of a processor on a number of small
test programs.

## Task Description

Your task is to complete the provided skeleton MIPS instruction set
simulator written in C such that it covers enough of the MIPS
instruction set to run the provided example programs.

For this task, you need to work predominantly on the so called
fetch-decode-execute loop, which forms the core of the MIPS interpreter.
In this loop, MIPS instruction in their binary encoded format are
fetched from memory, decoded to isolate opcodes from register fields,
immediate values and other instruction fields, and then a sequence of
operations (equivalent to the semantic action of the decoded MIPS
instruction) is performed on data structures representing the MIPS
register set and memory. The effect is that each instruction as it is
encountered as part of the programme execution is being fetched, decoded
and executed, before the programme counter (PC) is being updated and the
fetch-decode-execute cycle is performed again for the next instruction.
Execution only stops when the relevant system call (exit) is being
encountered.

While arithmetic/logic instructions read from and write to registers
while performing their arithmetic or logic operations, memory
instructions transfer values to and from memory. Control flow
instructions (such as jump) modify the PC. A special class of
instructions are system calls, which can be used for I/O, memory
management or programme termination (in a real system these system calls
would be handled by the Operating System, which would then provide the
relevant service to the user application).

You don't need to implement the entire MIPS instruction set, but for
this coursework a subset of MIPS instruction is sufficient. The required
subset of instructions for you to implement is determined by the
provided MIPS example programs - you should implement those instructions
required to run those examples!

You will most likely implement one instruction at a time, i.e. once you
have identified the next instruction to add to your simulator, you would
need to add the code to decode the instruction in a large, nested
switch-case statement. Then you will have to add a small piece of code
to represent the actions of the MIPS instructions, i.e. its effects on
registers and memory. As you will certainly want to test your
instructions, you can add "tracing" code that prints out the decoded
instructions. You can do this by passing ``-DDEBUG`` to the compiler, which
can be done by uncommenting the following line in the provided Makefile
(and commenting out the current ``CFLAGS`` line).

``CFLAGS := -O2 -Wall -Wextra -DDEBUG``

When you then rebuild your program (''make clean'' followed by ''make''), and
then run e.g. the "Hello World" program, you should see a program trace
like this, where the first column represents the current PC, the second
column the current MIPS instruction in hexadecimal encoding, and the
third column the disassembled, i.e. decoded instruction as MIPS
assembly. You can see instructions as they are being executed! After the
syscall instruction you see the output of the program, "Hello World",
interleaved with the instruction trace. 

#### Note: The current autograder tests only check output and the register dumps. The autograder will be updated with more tests to check for debug traces.

    [0x00400000] 0x8FA40000 lw $4, 0($29)
    [0x00400004] 0x27A50004 addiu $5, $29, 4
    [0x00400008] 0x24A60004 addiu $6, $5, 4
    [0x0040000C] 0x00041080 sll $2, $4, 2
    [0x00400010] 0x00C23021 addu $6, $6, $2
    [0x00400014] 0x0C100068 jal 0x4001a0
    [0x004001A0] 0x24020004 addiu $2, $0, 4
    [0x004001A4] 0x3C011001 lui $1, 4097
    [0x004001A8] 0x34240000 ori $4, $1, 0
    [0x004001AC] 0x0000000C syscall
    Hello World
    [0x004001B0] 0x3C011000 lui $1, 4096
    [0x004001B4] 0x8C290000 lw $9, 0($1)
    [0x004001B8] 0x03E00008 jr $31
    [0x00400018] 0x00000000 nop
    [0x0040001C] 0x2402000A addiu $2, $0, 10
    [0x00400020] 0x0000000C syscall

Please also make sure your instruction set simulator prints a "register
dump" at the end. For example, once the "Hello World" program has been
terminated, your simulator would dump the values of register 0-31 as
follows:

    $0  : 0x00000000
    $1  : 0x10000000
    $2  : 0x0000000a
    $3  : 0x00000000
    $4  : 0x10010000
    $5  : 0x7ffff000
    $6  : 0x7ffff004
    $7  : 0x00000000
    $8  : 0x00000000
    $9  : 0x00000000
    $10 : 0x00000000
    $11 : 0x00000000
    $12 : 0x00000000
    $13 : 0x00000000
    $14 : 0x00000000
    $15 : 0x00000000
    $16 : 0x00000000
    $17 : 0x00000000
    $18 : 0x00000000
    $19 : 0x00000000
    $20 : 0x00000000
    $21 : 0x00000000
    $22 : 0x00000000
    $23 : 0x00000000
    $24 : 0x00000000
    $25 : 0x00000000
    $26 : 0x00000000
    $27 : 0x00000000
    $28 : 0x10008000
    $29 : 0x7fffeffc
    $30 : 0x00000000
    $31 : 0x004001b0

## Provided Framework and Files

The framework with various header and source for your instruction set
simulator is provided in the src directory within your coursework
folder.

Please note that the provided framework is incomplete, i.e. there are
gaps that you need to complete, before the files can be compiled
successfully.

-   **Header Files**

    -   **global.h** - This file include declarations of global data,
        e.g. the register file or the program counter (PC). You might
        also find this a convenient location to introduce some global
        macros. You might want to declare global data structures as
        extern here, whereas the actual definitions of global data
        might be located in the sim.c file.

    -   **loader.h** - This file includes the declaration of the
        read_file function, which is exported by the loader. It also
        contains useful data structure declarations and macros used by
        the loader. You won't need to modify this file, unless you
        want to extend its functionality.

    -   **memory.h** - This file includes declarations of useful
        functions to access MIPS memory, e.g. read_word and
        write_word. You won't need to modify this file, unless you
        want to extend its functionality.

    -   **interpreter.h** - This files includes the declaration of the
        run function, which is exported by the interpreter. You won't
        need to modify this file, unless you want to extend its
        functionality.

-   **Source Files**

    -   **loader.c** - This file contains the implementation of the
        loader, i.e. the read_file function. Don't make any changes
        here.

    -   **memory.c** - This file contains the implementations of the
        various memory access functions, e.g. read_word. You won't
        need to modify this file, unless you want to extend its
        functionality.

    -   **interpreter.c** - This file contains the implementation of
        the run function, which contains the fetch-decode-execute
        loop, which is central to the instruction set simulator. You
        will have to add a lot of functionality to this function for
        decoding MIPS instructions and their implementations. A few
        helper functions and a skeleton of the execution loop have
        been provided, but you need to complete this to cover the
        relevant instructions of the MIPS instruction set.

    -   **sim.c** - This file contain the main function of the
        instruction set simulator. While you don't need to make any
        changes to the code of the main function, you might want to
        include definitions of global variables e.g. for the register
        file here.

You can use the provided Makefile to build your simulator from sources
using make:

❯ ``make``

You should then see output like:

    mkdir -p build/./src/

    cc -I./src -MMD -MP -O2 -Wall -Wextra -c src/loader.c -o build/./src/loader.c.o
    mkdir -p build/./src/
    cc -I./src -MMD -MP -O2 -Wall -Wextra -c src/sim.c -o build/./src/sim.c.o
    mkdir -p build/./src/
    cc -I./src -MMD -MP -O2 -Wall -Wextra -c src/memory.c -o build/./src/memory.c.o
    mkdir -p build/./src/
    cc -I./src -MMD -MP -O2 -Wall -Wextra -c src/interpreter.c -o build/./src/interpreter.c.o
    cc ./build/./src/loader.c.o ./build/./src/sim.c.o ./build/./src/memory.c.o ./build/./src/interpreter.c.o -o build/sim

Assuming your code is ok and complete (as in this example), an
executable for your simulator will be generated in the build folder. It
will be called ``sim``.

Note that the ``Makefile`` is configured for the compiler to provide you
with warnings when it encounters problematic source code constructs that
might lead to incorrect or non-portable behaviours. Don't just ignore
these warnings! They are often an indicator of a problem with your code.
Read the warnings and error messages, try to work out what they want to
tell you, and fix your code accordingly.

## Assembling and Running MIPS examples

You can find a number of MIPS example programs that you can run on your
MIPS instruction set simulator. These example programs stored in the
examples folder.

Before you can run these programs you need to assemble them, i.e.
translate them from MIPS assembly code (which contains mnemonics ``lw`` or
``add``) to MIPS machine code (which is the binary encoding of those
instructions). You might find the resources in the section at the bottom
of this handout useful, in particular, the instruction set references
and the online MIPS instruction converter. However, you don't need to
worry about manually translating MIPS assembly to binary. This is the
job of the assembler, which is a useful software tool for this kind of
translation. In this coursework we make use of MARS to help us with this
task.

You can use the provided assemble.sh shell script to invoke MARS to not
only assemble a given MIPS assembly program, but this script will
additionally translate the startup code (in ``startup.s``) and I/O library
(in ``libio.s``), and then pack both the ``.text`` and ``.data`` sections into a
single file (called ``hexcode.txt``).

For example, to translate the "Hello World" example in
``examples/hello_world/hello.s`` you can run the following command:

❯ ``./assemble.sh examples/hello_world/hello.s``

This will generate a new file ``hexcode.txt``, which looks like this:

    .text
    :040000008FA40000C
    :0400040027A5000428
    :0400080024A6000426
    :04000C00000410805C
    :0400100000C23021D9
    :040014000C10006864
    :0400180000000000E4
    :04001C002402000AB0
    ....
    :0401B4008C29000092
    :0401B80003E0000858
    :00000001FF
    .data
    :040000006C6C654877
    :040004006F57206FA3
    :040008000A646C72A8
    :04000C0000000000F0
    :0400100000000000EC
    :0400140000000000E8
    :0400180000000000E4
    ...
    :040FF40000000000F9
    :040FF80000000000F5
    :040FFC0000000000F1
    :00000001FF

This representation of the original "Hello World" program will be the
input to your simulator!

There is another provided script ``assemble_and_run.sh``, which you can use
to assemble and run the provided example programs once you have
implemented your instruction set simulator.

For example, you can run the same "Hello World" program from before like
this:

❯ ``./assemble_and_run.sh examples/hello_world/hello.s``

resulting in output:

``Hello World``

You can compare this to MARS, which you can invoke from the command line
using the provided run_mars.sh script like this:

❯ ``./run_mars.sh examples/hello_world/hello.s``

resulting in the same output as before:

``Hello World``

As you can see, both MARS and our own simulator produce the same output
"Hello World" on the terminal, which is good.

Now have a look at the other MIPS examples, and try to assemble them.
You can also try running the examples using MARS as shown. Your own
simulator will not yet be able to run the programs successfully, but as
you complete this piece of coursework you should be able to run all of
the example programs using your simulator (using the
``assemble_and_run.sh``).

You can run the simulator in a number of different ways:

-   Use the provided ``assemble_and_run.sh`` script as described.

-   Use the make run command for a previously assembled MIPS program:

    ❯ ``make run FILE=hexcode.txt``
    
    resulting in
    
    ``Hello World``

-   Invoke the simulator binary directly as in:

    ❯ ``build/sim hexcode.txt``
    
    also resulting in
     
    ``Hello World``

## How to get started

While we appreciate that you may want to develop your code on your own
machines, however, your coursework submission will ultimately have to
run on an Informatics DICE machine, e.g. one of the machines in the
Appleton Tower Computing Labs. If you haven't used the Informatics DICE
environment before, the please check here and familiarise yourself with
DICE:

<https://opencourse.inf.ed.ac.uk/DICE-UG>

There are many ways to approach this coursework, which all involve steps
like this:

- Define the MIPS register file as a data structure and introduce
  convenient ways of accessing the registers for reading and writing.

- Introduce code in the main run loop to update the PC (as part of the
  fetch-decode-execute loop).

- Make the system calls work. This enables you to test basic I/O.

- Start with the simple MIPS examples, e.g. the "Hello World" or basic
  arithmetic examples. Explore which instructions need to be
  implemented, either by studying the provided MIPS example programs
  or running the simulator until it complains about a
  missing/unrecognised instruction

- Make use of the instruction traces (``DEBUG``) option to see which
  instructions are being executed. Compare against MARS in the MARS
  IDE.

- If needed, add additional ``DEBUG`` output to e.g. show values read from
  or written to registers. Debug where necessary.

- Once an example program is working, move to the next. Add new
  instructions as necessary.

- Think about corner cases. Study the instruction reference to find
  out what exactly the MIPS instructions are supposed to be doing.

## Subset of the MIPS Instruction Set

You don't need to implement the complete MIPS instruction set for this
piece of coursework. It is sufficient to provide implementations for
those instructions required by the provided example programs. If your
instruction set simulator has support for those instructions and can run
the provided MIPS example applications (in the example folder)
successfully, you are done!

Your instruction set simulator will only be tested against those
instructions needed by the example programs. The "hidden tests" won\'t
consider additional instructions except for the bonus tasks.

## Restrictions on the use of C Library Functions

For this coursework you are free to use any functions from the C
standard library as defined here
(<https://en.cppreference.com/w/c/header>).

You are \*not\* allowed to make use of any external libraries other than
the C standard library, though.

## How to get support

There is plenty of support available to you if you should get stuck or
have any questions about this coursework: The INF2C-CS lab demonstrators
can provide general and practical support if something doesn't work, or
you have general C or MIPS related questions. You can also drop in to
InfBase to get additional tutoring and support for your year 1/2
courses. Please take advantage of the labs and the lab demonstrators who
are there to answer your questions.

The INF2C-CS Piazza Forum is a great place to find support. If you have
any questions about the assignment, please start by checking existing
discussions on Piazza. If you can't find the answer to your question,
start a new discussion -- chances are, others have already encountered
(and, possibly, solved) the same problem. The TA and the course
instructor will also monitor Piazza and contribute to the discussions.

The scheduled INF2C-CS labs will be very busy in the week of
the deadline! While we will add extra demonstrators to account for this,
we cannot guarantee being able to help everyone that shows up to a given
session. You may be asked to return the next day! Therefore, we
encourage you to start with the coursework early and attend lab sessions
well in advance of the deadline.

## Academic Integrity and the Use of ChatGPT

Please remember the good scholarly practice requirements of the
University regarding work for credit. You can find guidance at the
School page:

<https://web.inf.ed.ac.uk/infweb/admin/policies/academic-misconduct>

This also has links to the relevant University pages.

You must work on this coursework on your own, but you can discuss
general ideas with other students without sharing actual code or
solutions. You are not allowed to use ChatGPT or other Generative AI
tools for this coursework (even if this might be tempting and chances
are that ChatGPT would be able to create a solution). Instead, if you
need help with the coursework, please make use of the support provided
to you! It is important for your learning that you develop the
knowledge, skills and understanding associated with the learning
outcomes of this coursework!

## Marking Criteria

Your programs will be primarily judged according to correctness,
completeness, and correct use of C concepts in your code base. We will
make extensive use of an automated testing framework to thoroughly check
your submission.

Commenting a program and keeping it tidy is very important. Make sure
that you comment the code throughout and format it neatly.

## Bonus Marks

While you can reach marks up to a standard of an "A" grade by completing
all the basic tasks of this coursework, you can earn "bonus marks" for
your coursework. Bonus marks are awarded in line with the "exceptional"
ranges of the Extended Common Marking Scheme, i.e. the range between
70-100%. Bonus marks are only awarded if your coursework is at least
satisfactory across all of the basic marking criteria! If you have
struggled with the basic part of this coursework, or have spent
significantly longer than 15 hours, please do not attempt these
additional tasks for bonus marks. It would be better use of your time
revising INF2C-CS materials, or engage with coursework for other
courses.

Here are some suggestions on how you can earn bonus marks. Most of these
additions are hard and/or require significant development effort.
Attempt these only if you have plenty of spare time! You don't need to
do any of these to achieve an "A" - this is for bonus marks only.

1.  Extend your simulator with the remaining user-level instructions and
    provide test cases that thoroughly exercise these added
    instructions. Think of corner cases. Your simulator should be able
    to run any MIPS user-level program now that does not depend on
    specific Operating System functionality beyond the MARS system
    calls.

2.  Extend your simulator to generate memory traces and integrate with
    the DineroIV (<https://github.com/atos-tools/dineroIV>) cache
    simulator. Your extended instruction set simulator will output
    detailed cache statistics for the program that it has executed.

## Submission Logistics

Once you have pushed all of your changes to GitHub and you are happy
with your code and your points, you are finished! At the deadline, we
will revoke your write access to this repository and grade your work.
The last version committed before the deadline is the one that will be
marked. Extensions or Extra Time Adjustments (ETA) are not permitted in
accordance with Rule 2.

## Useful Materials and References

You might find following materials and references useful for this
coursework:

- Patterson and Hennessy textbook, Appendix A \"Instruction Set
    Principles\"

- MIPS Instruction Converter: <https://www.eg.bucknell.edu/~csci320/mips_web/>

- MIPS Assembly: <https://en.wikibooks.org/wiki/MIPS_Assembly>

- MIPS Instruction Reference: <https://phoenix.goucher.edu/~kelliher/f2009/cs220/mipsir.html>

- Encoding of MIPS Instructions - you might find the opcode map in figure A.10.2 very useful: <https://www.math.unipd.it/~sperduti/ARCHITETTURE-1/mips32.pdf>

- Another MIPS encoding reference: <https://student.cs.uwaterloo.ca/~isg/res/mips/opcodes>

- MIPS encoding and decoding instructions: <https://marz.utk.edu/my-courses/cosc130/lectures/encoding-and-decoding-instructions/>

- A simple RISC-V emulator in C: <https://fmash16.github.io/content/posts/riscv-emulator-in-c.html>
