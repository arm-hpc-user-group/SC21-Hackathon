# Compiler Comparison with Naive Matrix Multiplication

This version of the code has a naive implementation of a 
matrix-matrix multiplication. The binary is run with three 
command line options, which represent the three dimensions 
of the matrices $`n`$, $`m`$, and $`l`$.  This then represents the 
following matrix operation:

```math
C_{nl} = A_{nm} \times B_{ml}
```

To compile the code, simply type:

> `make`

This will build three versions of the executable:
 * *_def.exe: Compilation without any optimizing compiler flags.
 * *_opt.exe: Compilation with a high level of optimization.
 * *_novec.exe: Compilation with a high level of optimization, but with automatic vectorization disabled.

To run all three versions, type:

> `make run`

It's a good idea to clean between builds and runs.  Use the _clean_ make target
to remove build products:

> `make clean`

Try building with different compilers and notice the relative performance of 
each compiler.  Remember that **this is a naive, unoptimized code.** In the 
next steps, we will explore how to improve on performace.

