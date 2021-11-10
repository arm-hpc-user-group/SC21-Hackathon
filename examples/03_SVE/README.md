# SVE Programming Examples

This directory contains examples of hand-coded SVE and SVE2 code, either
in Assembly or in C with the Arm C Language Extensions (SVE intrinsics).
A corresponding C reference code is provided for functional correctness.

These examples were copied from developer.arm.com[^1] and updated to 
separate SVE from SVE2 and better support native compilation on CPUs that 
implement SVE or SVE2. The whitepaper 
[SVE-SVE2-programming-examples-REL-01.pdf](https://developer.arm.com/documentation/dai0548/b) 
describes each example in detail
and walks through the SVE features each example highlights.  You are
encouraged to read through this document while working with these examples.


## Usage

Each example includes a makefile to compile and run the code. Run the 
command `make` in each of the example directories to create an executable 
in the `build` subdirectory.  Type `make run` to compile and run the code. 
Run `make clean` to remove build products.  For example:

```bash
cd B5.1_sve_matmul_f64
make 
make run
make clean
```


## SVE vs SVE2

SVE is the first generation of Arm's Scalable Vector Extension.  It introduces
Arm's vector lenght agnostic programming approach and provides many powerful
features to facilitate vectorization in complex codes.  Some of these features
are reduction instructions for a number of arithmetic operations, extensive 
load/store instruction support, predicate and loop control support, and logical 
and bitwise instructions support. The SVE instruction set also provides thorough 
floating-point arithmetic support, and basic integer arithmetic support.

SVE2, the Scalable Vector Extension v2, is a superset of the Armv8-A SVE with 
expanded functionality. The SVE2 instruction set adds features that benefit 
media processing workloads, ML workloads, and generally broaden the applicability 
of SVE beyond high performance computing applications.

Because SVE2 is a superset of SVE, CPUs that implement SVE2 can run SVE binaries
without any modifications.  However, CPUs that only implement SVE (such as the
Fujitsu A64FX) will not be able to run SVE2 binaries.  Attempting to run an SVE2
binary on such a CPU will generate an Illegal Instruction error.  

Use the Arm Instruction Emulator (ArmIE) to get started with SVE2 ahead of hardware 
availability.


[^1]: https://developer.arm.com/documentation/dai0548/latest
