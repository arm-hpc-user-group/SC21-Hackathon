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

# FX700 energy units in nano-Joules
EA_CORE_U=8.04
EA_L2_U=32.8
EA_MEM_U=271

ITERATIONS = 1000

SRC = GravityForceKernel.cpp main.cpp

EXE_NEON   = hacc_$(COMPILER)_neon.exe
EXE_SVE    = hacc_$(COMPILER)_sve.exe

EXE = $(EXE_NEON) $(EXE_SVE)


.PHONY: all clean run

all: $(EXE)

clean:
	rm -f *.o *.exe *.opt.yaml

run: $(EXE)
	$(call print_hline)
	./$(EXE_NEON) $(ITERATIONS)
	$(call print_hline)
	./$(EXE_SVE) $(ITERATIONS)
	$(call print_hline)

run-perf: $(EXE)
	# r11:   CPU_CYCLES: This event counts every cycle
	# r1e0:  EA_CORE: This event counts energy consumption per cycle of core.
	# r3e0:  EA_L2: This event counts energy consumption per cycle of L2 cache.
	#        It counts all events caused in measured CMG regardless of measured PE.
	# r3e8:  EA_MEMORY: This event counts energy consumption per cycle of CMG local memory.
	#        It counts all events caused in measured CMG regardless of measured PE.
	perf stat -x\; -o neon.perf -e duration_time,r11,r1e0,r3e0,r3e8 ./$(EXE_NEON) $(ITERATIONS)
	perf stat -x\; -o sve.perf -e duration_time,r11,r1e0,r3e0,r3e8 ./$(EXE_SVE) $(ITERATIONS)
	./postproc_perf_energy.py neon.perf -c $(EA_CORE_U) -l $(EA_L2_U) -m $(EA_MEM_U)
	./postproc_perf_energy.py sve.perf -c $(EA_CORE_U) -l $(EA_L2_U) -m $(EA_MEM_U)

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
