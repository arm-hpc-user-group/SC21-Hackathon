# The Arm C Language Extensions for SVE

Intrinsics are C or C++ pseudo-function calls that the compiler replaces with 
the appropriate SIMD instructions. This lets you use the data types and 
operations available in the SIMD implementation, while allowing the compiler 
to handle instruction scheduling and register allocation. These intrinsics are 
defined in [the Arm C Language Extensions for SVE specification](https://developer.arm.com/docs/100987/latest/arm-c-language-extensions-for-sve).

The Arm C language extensions for SVE provide a set of types and accessors for 
SVE vectors and predicates, and a function interface for all relevant SVE 
instructions. The function interface is more general than the underlying 
architecture, so not every function maps directly to an architectural 
instruction. The intention is to provide a regular interface and leave the 
compiler to pick the best mapping to SVE instructions. The Arm C Language 
Extensions for SVE specification has a detailed description of this interface, 
and must be used as the primary reference. This section introduces a selection 
of features to help you get started with the Arm C Language Extensions (ACLE) 
for SVE.  For more details see [the Arm Compiler Scalable Vector Extension User Guide](https://developer.arm.com/documentation/100891/0612/coding-considerations/using-sve-intrinsics-directly-in-your-c-code).

## Usage

Each example includes a makefile to compile and run the code. Run the 
command `make` in each of the example directories to create an executable.  
Type `make run` to compile and run the code. Run `make clean` to remove build products.  
For example:

```bash
cd 04_dotprod_complex
make 
make run
make clean
```

