# Math Libraries for Matrix Multiplication

Generally, it's bad practice to write your own matrix multiplication kernel. 
This operation has been well studied for decades and there are many extremely 
well-tuned implementations available.  Vendor-provided math libraries are 
usually the best choice, but open source packages like BLIS or OpenBLAS often 
deliver excellent performance as well.

In this version of the matrix multiplication kernel, we have replaced our
hand-written code with a call to a math library.

To compile run the code, simply type:

> `make`

You can specify a compiler and/or math library by typing:

> `make [COMPILER=compiler_name] [LIBRARY=library_name]`

The optional COMPILER and LIBRARY variables are used to specify the compiler
and math library you would like to use. For example:

> `make COMPILER=arm`

This will compile using the Arm Compiler for Linux and perform the matrix
multiplication by calling DGEMM in the Arm Performance Library. You could
also try:

> `make COMPILER=gnu LIBRARY=armpl`

This will compile with the GNU compiler and use the Arm Performance Library
to perform the DGEMM.  Vendor compilers and libraries are also supported.
For example:

> `make COMPILER=fujitsu`

This will use Fujitsu's SSL II math library to perform matrix multiplication.
Type `make COMPILER=help` to see all supported compilers, or type 
`make LIBRARY=help` to see supported libraries.

It's a good idea to clean between builds and runs.  Use the _clean_ make target
to remove build products:

> `make clean`



