# Arm SVE Compilers Tutorial

In this tutorial, we will work with various compilers to explore 
how each compiler works and how to get the most out of it. We will 
use a simple matrix-matrix multiplication code since it is a common 
motif that presents opportunities to show the relative performance 
of different compiler flags and scientific computing libraries.

We begin with a naive implementation of the kernel and show how compiler
flags and optimized math libraries can be used to dramatically improve
the kernel's runtime performance.  Each step of this tutorial improves
on the performance of the previous step, so it's a good idea to take
them in order.

## Comparing Compiler Performance

Different compilers optimize different code differently, so there will never
be one compiler to rule them all.  You may need to try several compilers to
find the one compiler that works best for your code.  To easily compare the
performance of different compiers, add *COMPILER=_COMPILER_NAME_* to the make
command line.  For example, to compile with Arm compilers:

> `make all COMPILER=arm`

Type `make COMPILER=help` to see all supported compilers.

## Comparing Library Performance

This tutoral also introduces math libraries as a way to get the best 
performance from codes that rely on common operations like matrix-matrix 
multiplication.  To easily compare the performance of different libraries,
add *LIBRARY=_LIBRARY_NAME_* to the make command line.  For example, to use
the Arm Performance Library with GNU compilers:

> `make all COMPILER=GNU LIBRARY=armpl`

Type `make LIBRARY=help` to see all supported math libraries.