# Measuring A64FX energy consumption

This example demonstrates how to measure energy consumption on the A64FX.  Energy data is
gathered from hardware performance counters via `perf` and then postprocessed in a Python 
script to calculate power in Watts.  The same proceedure could be used in more advanced
profiling tools to calculate energy consumption in a code region of interest.

## Hardware performance counters

The Fujitsu A64FX provides hardware performance counters for measuring the energy consumption
at the core, L2 cache, and HBM.  The counters are listed in the A64FX documentation and can be
summarized as:

 - r1e0:  EA_CORE: This event counts energy consumption per cycle of core.
 - r3e0:  EA_L2: This event counts energy consumption per cycle of L2 cache. 
          It counts all events caused in measured CMG regardless of measured PE.
 - r3e8:  EA_MEMORY: This event counts energy consumption per cycle of CMG local memory.
          It counts all events caused in measured CMG regardless of measured PE.

The energy consumption of a single core can be derived from EA_CORE values.  L2 and HBM
are shared across all cores in a CMG, so the energy consumption of those components is 
reported for the full CMG, not per-core.

The PMU values must be scaled to calculate power consumption in Watts.  The scaling factors are:

 - EA_CORE: 8.04
 - EA_L2: 32.80
 - EA_MEM: 271.00

## Example output

Use `make run-perf` to run the example.  This builds two versions of the HACC GravityForce kernel.
One version uses NEON vectorization, while the other uses SVE.

```
[johlin02@pacu13 03_energy]$ make run-perf
# r11:   CPU_CYCLES: This event counts every cycle
# r1e0:  EA_CORE: This event counts energy consumption per cycle of core.
# r3e0:  EA_L2: This event counts energy consumption per cycle of L2 cache.
#        It counts all events caused in measured CMG regardless of measured PE.
# r3e8:  EA_MEMORY: This event counts energy consumption per cycle of CMG local memory.
#        It counts all events caused in measured CMG regardless of measured PE.
perf stat -x\; -o neon.perf -e duration_time,r11,r1e0,r3e0,r3e8 ./hacc_arm_neon.exe 1000
Maximum OpenMP Threads: 48
Iterations: 1000
Gravity Short-Range-Force Kernel (5th Order): 12823.6 -444.108 -645.349: 1.51121 s
perf stat -x\; -o sve.perf -e duration_time,r11,r1e0,r3e0,r3e8 ./hacc_arm_sve.exe 1000
Maximum OpenMP Threads: 48
Iterations: 1000
Gravity Short-Range-Force Kernel (5th Order): 12823.6 -444.108 -645.349: 1.45336 s
./postproc_perf_energy.py neon.perf -c 8.04 -l 32.8 -m 271
Core freq: 1.99784 GHz
Per-core power: 2.1054 Watt
Per-CMG L2 power: 1.93873 Watt
Per-CMG HBM power: 1.85643 Watt
./postproc_perf_energy.py sve.perf -c 8.04 -l 32.8 -m 271
Core freq: 1.99829 GHz
Per-core power: 2.12486 Watt
Per-CMG L2 power: 1.94959 Watt
Per-CMG HBM power: 1.85812 Watt
```
