#------------------------------------------------------------------------------------
# BSD 3-Clause License
# 
# Copyright (c) 2021, esiegmann
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#------------------------------------------------------------------------------------


# close binding since we want to see effect of loading a single socket
OMPENV = OMP_PLACES=CORES OMP_PROC_BIND=close OMP_NUM_THREADS=48 
#RUNCMD = srun -N 1 -p short -c 48

run:
	@echo "Please use the appropriate run_*.sh script"

# GCC 10.2.0 or older
gcc-old:
	which g++
	g++ --version
	g++ --std=c++17 -Ofast -ffast-math -Wall -mtune=native -mcpu=native  -march=armv8.2-a+sve -fopt-info-vec -fopt-info-vec-missed -fopenmp test.cc
	$(OMPENV) $(RUNCMD) ./a.out | tee out.`which g++ | sed -e s,/,:,g`

# GCC 10.2.1 or later understands a64fx target
gcc:
	which g++
	g++ --version
	g++ -Ofast -ffast-math -Wall -mtune=a64fx -mcpu=a64fx  -march=armv8.2-a+sve -fopt-info-vec -fopt-info-vec-missed -fopenmp test.cc
	$(OMPENV) $(RUNCMD) ./a.out | tee out.`which g++ | sed -e s,/,:,g`

# ARM clang
arm:
	which armclang++
	armclang++ --version
	armclang++ --std=c++17 -Ofast -ffp-contract=fast -ffast-math -Wall  -Rpass=loop-vectorize -march=armv8.2-a+sve -mcpu=a64fx -armpl -fopenmp test.cc
	$(OMPENV) $(RUNCMD) ./a.out | tee out.`which armclang++ | sed -e s,/,:,g` 

# CRAY
cray:
	which CC
	CC --version
	CC -O3 -h aggress,flex_mp=tolerant,msgs,negmsgs,vector3,omp test.cc
	$(OMPENV) $(RUNCMD) ./a.out | tee out.`which CC | sed -e s,/,:,g`

# Fujitsu A64FX compiler
fujitsu:
	which FCC
	FCC --version
	FCC -Kfast -Kopenmp test.cc
	$(OMPENV) $(RUNCMD) ./a.out | tee out.`which FCC | sed -e s,/,:,g`

# Intel Skylake
intel:
	which icpc
	icpc --version
	icpc -xHOST -O3 -ipo -no-prec-div -fp-model fast=2 -qopt-report=5  -qopt-report-phase=vec -mkl=sequential -qopt-zmm-usage=high -qopenmp test.cc
	$(OMPENV) ./a.out | tee out.`which icpc | sed -e s,/,:,g`

clean:
	/bin/rm -rf a.out *~ out.*

