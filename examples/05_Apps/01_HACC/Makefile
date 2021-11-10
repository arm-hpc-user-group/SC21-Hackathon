#------------------------------------------------------------------------------------
# Copyright (C) Arm Limited, 2019-2020 All rights reserved.
#
# The example code is provided to you as an aid to learning when working
# with Arm-based technology, including but not limited to programming tutorials.
#
# Arm hereby grants to you, subject to the terms and conditions of this Licence,
# a non-exclusive, non-transferable, non-sub-licensable, free-of-charge copyright
# licence, to use, copy and modify the Software solely for the purpose of internal
# demonstration and evaluation.
#
# You accept that the Software has not been tested by Arm therefore the Software
# is provided "as is", without warranty of any kind, express or implied. In no
# event shall the authors or copyright holders be liable for any claim, damages
# or other liability, whether in action or contract, tort or otherwise, arising
# from, out of or in connection with the Software or the use of Software.
#------------------------------------------------------------------------------------

include ../../config.mk

ITERATIONS = 1000

SRC = GravityForceKernel.cpp main.cpp

EXE_NEON   = hacc_$(COMPILER)_neon.exe
EXE_SVE    = hacc_$(COMPILER)_sve.exe

EXE = $(EXE_NEON) $(EXE_SVE)

# Runtime parameters
OMP_ENV = OMP_NUM_THREADS=48 OMP_PROC_BIND=close
NUMACTL = numactl -C 0-47 -l
RUN = $(OMP_ENV) $(NUMACTL)


.PHONY: all clean run

all: $(EXE)

clean:
	rm -f *.o *.exe *.opt.yaml

run: $(EXE)
	$(call print_hline)
	$(RUN) ./$(EXE_NEON) $(ITERATIONS)
	$(call print_hline)
	$(RUN) ./$(EXE_SVE) $(ITERATIONS)
	$(call print_hline)

$(EXE_NEON): $(SRC)
	$(call print_hline)
	$(call print_version)
	$(CXX) $(CXXFLAGS_REPORT) $(CXXFLAGS_OPT) $(CXXFLAGS_NEON) $(CXXFLAGS_OPENMP) -o $@ $^
	$(call print_hline)

$(EXE_SVE): $(SRC)
	$(call print_hline)
	$(call print_version)
	$(CXX) $(CXXFLAGS_REPORT) $(CXXFLAGS_OPT) $(CXXFLAGS_OPENMP) -o $@ $^
	$(call print_hline)
