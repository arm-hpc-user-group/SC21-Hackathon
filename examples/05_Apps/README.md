# SVE Application Examples

These are examples of simple HPC applications that can take advantage of SVE.

## HACC

The [HACCKernels benchmark](https://xgitlab.cels.anl.gov/hacc/HACCKernels) represents 
the computationally intensive part of an N-body cosmology code.  Application performance
is dominated by a long chain of floating point instructions in the innermost loop.
Application performance scales well with vector length since increasing vector width
increases the number of innermost loop iterations that can be completed simultaneously.

The figure of merit is wall clock time spent in the application loop.  It is reported
in seconds when the application exits.

Arm Compiler for Linux 20.3 on A64FX
```
------------------------------------------------
./hacc_arm_neon.exe 1000
Maximum OpenMP Threads: 48
Iterations: 1000
Gravity Short-Range-Force Kernel (5th Order): 12823.6 -444.108 -645.349: 5.19743 s
------------------------------------------------
./hacc_arm_sve.exe 1000
Maximum OpenMP Threads: 48
Iterations: 1000
Gravity Short-Range-Force Kernel (5th Order): 12823.6 -444.108 -645.349: 1.55594 s
------------------------------------------------
```
