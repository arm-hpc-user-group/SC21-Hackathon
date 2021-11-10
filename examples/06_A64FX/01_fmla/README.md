# Using FMLA to Measure Peak Performance

This example measures the wallclock time of a tight loop of fused multiply-add
(FMLA) instructions for 1,000,000 iterations.  The code is written in Assembly,
so the exact number of giga operations (GOP) is known.  Performance in gigaflops 
(GFLOPS) is simply `GFLOPS = GOP / SECONDS`.

Three versions are included:
 * `fmla-neon128.cc`: Loop over 16 NEON FMLA instructions, unrolled four times.
 * `fmla-sve512.cc`: Loop over 16 SVE FMLA instructions, unrolled four times.
 * `fmla-a64fx.cc`: An optimized version for the Fujitsu A64FX.  
   Loop over 20 SVE FMLA instructions, unrolled three times.

Running the three different versions will demonstrate the performance impact of
different vector instructions and instruction latencies.  For instance, the long
latency of the FMLA instruction on the Fujitsu A64FX requres a larger inner-loop 
body.  The "bubble" at the end of the A64FX pipe is easily visualized by
`vimdiff fmla-sve512.cc fmla-a64fx.cc`.

Theoretical double precision peak performance in gigaflops of any system can 
be calculated as:

```
[SIMD bits] / sizeof(double) * [SIMD per thread] * [ops per instruction] * [core frequency]
```

For example, on a 3.0 GHz CPU with two 128-bit SIMD units per thread 
with support for fused multiply-add instructions:

```
128 / 64 * 2 * 2 * 3.0 = 24 GFlops
```

Use this formula and this code to demonstrate the maximum performance of your
system as a fraction of theoretical peak.

## Usage

Type `make` to compile.  `make run` will run all three versions of the example.
`make run-perf` will run all three examples with some pre-selected performance
counters that are useful on the Fujitsu A64FX.

Example output from a Fujitsu A64FX:

```
------------------------------------------------
./fmla_neon128.exe
256000000 Flops in 0.0194543 seconds
13.1591 GFlops
------------------------------------------------
./fmla_sve512.exe
1024000000 Flops in 0.0194653 seconds
52.6063 GFlops
------------------------------------------------
./fmla_a64fx.exe
960000000 Flops in 0.0150295 seconds
63.8742 GFlops
------------------------------------------------
```

