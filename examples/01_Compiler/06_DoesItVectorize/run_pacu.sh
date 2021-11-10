#!/bin/bash

module purge
module load Generic-AArch64/RHEL/8/gcc/10.2.0
make gcc-old

module purge
module load Generic-AArch64/RHEL/8/gcc/11-ecd700c1
make gcc

module purge
module load A64FX/RHEL/8/arm-linux-compiler-20.3/armpl/20.3.0
make arm

module purge
module load arm21/21.0
make arm

module purge
module load A64FX/RHEL/8.1/FJSVstclanga/1.1.0
make fujitsu


./compare.py 0 out.*
