# NAS Parallel Benchmarks

This directory contains the OpenMP version of NPB 3.4.1.
The build system has been modified to accept the `COMPILER=`
and `LIBRARY=` make flags, and to name the target binary
after the compiler used to build it.

## Quick start

```
make COMPILER=arm
./bin/bt.arm.A.x
```

See README.npb for more information.
