OpenMP for thread-parallel matrix multiplication
================================================

Many math libraries can use multiple threads to take advantage of the many
cores available in modern CPUs.  In this example, we will use OpenMP to
enable multi-threading inside various math libraries and compare performance.

A parallelized version of the cache blocked kernel is also provided. This
version uses the OpenMP "collapse" clause to increase the parallel iteration
count distributed among threads.  This enables higher thread counts and
better supports the large number of cores found on modern CPUs.

To compile run the code, simply type:

`make [COMPILER=compiler_name] [LIBRARY=library_name]`

The optional COMPILER and LIBRARY variables are used to specify the compiler
and math library you would like to use.  For example:

`make COMPILER=arm`

This will compile using the Arm Compiler for Linux and perform the matrix
multiplication by calling DGEMM in the Arm Performance Library. You could
also try:

`make COMPILER=gnu LIBRARY=armpl`

This will compile with the GNU compiler and use the Arm Performance Library
to perform the DGEMM.  Vendor compilers and libraries are also supported.
For example:

`make COMPILER=fujitsu`

This will use Fujitsu's SSL II math library to perform matrix multiplication.

It's a good idea to clean between builds and runs.  Use the _clean_ make target
to remove build products:

`make clean`



