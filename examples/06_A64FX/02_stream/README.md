# Measuring bandwith with STREAM

The STREAM benchmark is a defacto standard for measuring memory bandwidth.
The benchmark includes four kernels: COPY, SCALE, ADD, and TRIAD. The kernels are executed in sequence in a loop.  Two parameters configure STREAM:
 * `STREAM_ARRAY_SIZE`: The number of double-precision elements in each array.
   It is critical to select a sufficiently large array size when measuring 
   bandwidth to/from main memory.
 * `NTIMES`: The number of iterations of the test loop.

See *STREAM_README* in this directory for more details on STREAM.

This example is structured to demonstrate the performance benefits of OpenMP,
effective use of NUMA, and features of the Arm architecture.  It uses a version of
stream that has been modified to enable dynamic memory allocation and to separate 
the kernel implementation from the benchmark driver.  This makes it the code easier
to read and faciliatates the use of external tools to measure the performance in 
each kernel.

## 01_stream_vanilla

This is a basic, untuned, out-of-box, "vanilla" implementation of STREAM. 
Performance will most likely be very poor since it uses only a single core and 
does not consider NUMA or any architectural features. 

gcc 11 on the Fujitsu A64FX:
```
-------------------------------------------------------------
Function    Best Rate MB/s  Avg time     Min time     Max time
Copy:           40859.3     0.003931     0.003916     0.003981
Scale:          40796.5     0.003931     0.003922     0.003949
Add:            47235.1     0.005109     0.005081     0.005188
Triad:          47253.3     0.005096     0.005079     0.005114
-------------------------------------------------------------
```

## 02_stream_openmp

A moderately tuned version of STREAM that uses OpenMP to span multiple threads 
and numactl to improve memory/thread locality.  On many systems, this implementation 
of STREAM will be close to 80% of the theoretical peak bandwidth.

gcc 11 on the Fujitsu A64FX:
```
-------------------------------------------------------------
Function    Best Rate MB/s  Avg time     Min time     Max time
Copy:          537948.0     0.032011     0.031936     0.032123
Scale:         537695.1     0.032026     0.031951     0.032179
Add:           597172.3     0.043259     0.043153     0.043500
Triad:         597324.1     0.043282     0.043142     0.044186
-------------------------------------------------------------
```

## 03_stream_triad

A cut-down version of STREAM that only invokes the TRIAD kernel.  This version 
is useful for viewing particular performance counters and iterating over optimization 
strategies for a target platform.

gcc 11 on the Fujitsu A64FX:
```
-------------------------------------------------------------

 Performance counter stats for './stream_triad_gnu.exe':

    47,423,184,916      r11
       562,779,776      r23
    39,571,623,033      r24
    37,426,955,843      r184
       585,048,483      r191
     1,178,420,034      r192
     3,030,935,341      r193
     2,399,010,058      r194

       0.643607455 seconds time elapsed

      19.007594000 seconds user
       4.712299000 seconds sys
```

## 04_stream_zfill

An optimized version of STREAM that uses Arm's `DC ZVA` instruction to zero-fill
cache lines.  `DC ZVA` isn't a prefetch instruction, but rather a way to map cache 
lines to virtual memory without having to read main memory.  Many architectures 
(not just Arm's) load a whole cache line from main memory when any address in the 
line is written to.  This is so that when the cache line is flushed the addresses 
adjacent to the written address retain their values.  On Arm, if you know for certain 
that the entire cache line will be written before it is flushed (or you you want to 
write zeros to main memory in any case), you can use the "DC ZVA" instruction to map 
the cache line to the appropriate virtual address without first reading main memory.  
All values in the cache line will be set to zero.  

This optimization can dramatically improve the performance of systems with wide L2 
cache lines and/or very little available L3 cache.  The Fujitsu A64FX has no L3, and 
each L2 cache is 256 bytes, so a "useless" load of the destination array from main 
memory has a much higher penalty on A64FX.

gcc 11 on the Fujitsu A64FX:
```
-------------------------------------------------------------
Function    Best Rate MB/s  Avg time     Min time     Max time
Copy:          780579.1     0.022083     0.022009     0.022202
Scale:         780689.0     0.022146     0.022006     0.022576
Add:           788330.3     0.032902     0.032689     0.033698
Triad:         787974.0     0.032808     0.032704     0.033263
-------------------------------------------------------------
```

## 05_stream_fujitsu

The Fujitsu compiler for A64FX can automatically generate the `DC ZVA` zfill
instructions.  It also uses a more highly-tuned memory management environment and
OpenMP runtime that can take advantage of the A64FX's hardware barrier. This
version of STREAM should achieve the best bandwidth and will do it without inline
assembly, but it does require the Fujitsu compiler.

fcc 4.2.1 on Fujitsu A64FX:
```
-------------------------------------------------------------
Function    Best Rate MB/s  Avg time     Min time     Max time
Copy:          755455.3     0.022814     0.022741     0.022887
Scale:         768704.5     0.022393     0.022349     0.022464
Add:           819550.3     0.031496     0.031444     0.031545
Triad:         815555.5     0.031672     0.031598     0.031754
-------------------------------------------------------------
```

## 06_stream_zfill_acle

Further optimization of the 04_stream_zfill vesion that uses SVE intrinsics via the 
Arm C Language Extensions (ACLE).  The memory bandwidth performance of this version 
does not improve, but the number of front-end bound cyles is reduced due to more 
effective vectorization of the inner loop.  Since STREAM is backend-bound on main
memory, there is no gain.

Arm Compiler for Linux 20.3 on Fujitsu A64FX:
```
-------------------------------------------------------------
Function    Best Rate MB/s  Avg time     Min time     Max time
Copy:          729261.4     0.023643     0.023558     0.023709
Scale:         707515.2     0.024440     0.024282     0.025103
Add:           768215.5     0.033778     0.033545     0.034639
Triad:         781942.8     0.033090     0.032956     0.033276
-------------------------------------------------------------
```

## 99_stream

An omnibus edition of STREAM for porting, profiling, and tuning on new platforms.
Examples 1-6 were derived from this code.


