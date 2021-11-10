# Ookami Compiler Auto-vectorization Tests

This code was contributed by Eva Siegmann and Robert Harrison of 
Stony Brook University.  The original code can be found at Eva's
Github [1].

## Example Output

```
make[1]: Leaving directory '/home/johlin02/arm-sve-tools/01_Compiler/06_DoesItVectorize'
make[1]: Entering directory '/home/johlin02/arm-sve-tools/01_Compiler/06_DoesItVectorize'
which FCC
/opt/FJSVstclanga/v1.1.0/bin/FCC
FCC --version
FCC (FCC) 4.2.1 20200821
simulating gcc version 6.1
Copyright FUJITSU LIMITED 2019-2020
FCC -Kfast -Kopenmp test.cc
OMP_PLACES=CORES OMP_PROC_BIND=close OMP_NUM_THREADS=48   ./a.out | tee out.`which FCC | sed -e s,/,:,g`
# threads         simple           gather          scatter      shortgather     shortscatter        predicate       reciprocal             sqrt              exp              sin              pow
# -------  ---------------  ---------------  ---------------  ---------------  ---------------  ---------------  ---------------  ---------------  ---------------  ---------------  ---------------
     0          1.43e-10         4.17e-10         9.44e-10         2.98e-10         7.01e-10         1.30e-10         2.85e-10         5.28e-10         1.05e-09         1.21e-09         4.26e-09
     1          1.41e-10         4.37e-10         9.27e-10         3.97e-10         7.18e-10         1.30e-10         2.85e-10         5.27e-10         1.05e-09         1.21e-09         4.27e-09
     4          1.42e-10         4.44e-10         9.54e-10         4.09e-10         7.17e-10         1.30e-10         2.85e-10         5.28e-10         1.05e-09         1.21e-09         4.28e-09
     8          1.41e-10         4.42e-10         9.49e-10         4.13e-10         7.17e-10         1.30e-10         2.86e-10         5.27e-10         1.05e-09         1.21e-09         4.27e-09
    12          1.46e-10         4.50e-10         9.79e-10         4.14e-10         7.27e-10         1.31e-10         2.92e-10         5.39e-10         1.07e-09         1.24e-09         4.35e-09
    24          1.45e-10         4.49e-10         9.84e-10         4.13e-10         7.27e-10         1.33e-10         2.92e-10         5.38e-10         1.07e-09         1.24e-09         4.35e-09
    36          1.45e-10         4.50e-10         9.98e-10         4.14e-10         7.27e-10         1.33e-10         2.93e-10         5.38e-10         1.08e-09         1.23e-09         4.35e-09
    48          1.45e-10         4.51e-10         1.04e-09         4.19e-10         7.27e-10         1.33e-10         2.92e-10         5.39e-10         1.07e-09         1.24e-09         4.35e-09
make[1]: Leaving directory '/home/johlin02/arm-sve-tools/01_Compiler/06_DoesItVectorize'

Compiler   0: /opt/FJSVstclanga/v1.1.0/bin/FCC
Compiler   1: /opt/arm/arm-linux-compiler-20.3_Generic-AArch64_RHEL-8_aarch64-linux/bin/armclang++
Compiler   2: /opt/arm/arm-linux-compiler-21.0_Generic-AArch64_RHEL-8_aarch64-linux/bin/armclang++
Compiler   3: /opt/pub/Generic-AArch64/RHEL/8/gcc/10.2.0/bin/g++
Compiler   4: /opt/pub/Generic-AArch64/RHEL/8/gcc/11-ecd700c1/bin/g++

                          0          1          2          3          4
----------------------------------------------------------------------
         simple:   1.43e-10   2.18e-10   2.21e-10   2.24e-10   2.25e-10
         gather:   4.17e-10   4.86e-10   4.81e-10   4.79e-10   4.77e-10
        scatter:   9.44e-10   1.07e-09   9.61e-10      1e-09   1.05e-09
    shortgather:   2.98e-10   3.42e-10   3.45e-10   3.46e-10   3.47e-10
   shortscatter:   7.01e-10   7.17e-10   7.15e-10   7.06e-10   7.18e-10
      predicate:    1.3e-10   3.04e-10   3.06e-10   2.11e-10   2.14e-10
     reciprocal:   2.85e-10   9.71e-09   7.59e-10   9.71e-09   9.71e-09
           sqrt:   5.28e-10   9.71e-09   9.71e-09   9.71e-09   9.71e-09
            exp:   1.05e-09   2.72e-09   2.33e-09    3.2e-08    3.2e-08
            sin:   1.21e-09   5.11e-09   3.64e-09   4.31e-08   4.28e-08
            pow:   4.26e-09   3.64e-08   3.77e-08   1.31e-07    1.3e-07
```

[1]: https://github.com/esiegmann/Ookami

