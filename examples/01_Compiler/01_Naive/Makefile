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

# Input parameters for `make run`
MATRIX_SIZE = 256 256 256


SRC = mm.cpp

EXE_DEF       = mm_$(COMPILER)_def.exe
EXE_OPT       = mm_$(COMPILER)_opt.exe
EXE_OPT_NOVEC = mm_$(COMPILER)_opt_novec.exe

EXE = $(EXE_DEF) $(EXE_OPT) $(EXE_OPT_NOVEC)


.PHONY: all clean run

all: $(EXE)

clean:
	rm -f *.o *.exe *.opt.yaml

run: $(EXE)
	$(call print_hline)
	./$(EXE_DEF) $(MATRIX_SIZE)
	$(call print_hline)
	./$(EXE_OPT) $(MATRIX_SIZE)
	$(call print_hline)
	./$(EXE_OPT_NOVEC) $(MATRIX_SIZE)
	$(call print_hline)

run-perf: $(EXE)
	# r8028: FP_FMA_SPEC: architecturally executed floating-point 
	#        fused multiply-add and multiply-subtract operations
	# r11:   CPU_CYCLES: This event counts every cycle
	# r23:   STALL_FRONTEND: every cycle counted by the CPU_CYCLES 
	#        event on that no operation was issued because there 
	#        are no operations available to issue for this PE from 
	#        the frontend.
	# r24:   STALL_BACKEND: every cycle counted by the CPU_CYCLES 
	#        event on that no operation was issued because the 
	#        backend is unable to accept any operations.
	# r191:  1INST_COMMIT: cycles where one instruction is committed.
	# r192:  2INST_COMMIT: cycles where two instructions are committed.
	# r193:  3INST_COMMIT: cycles where three instructions are committed.
	# r194:  4INST_COMMIT: cycles where four instructions are committed.
	$(call print_hline)
	perf stat -e r8028,r11,r23,r24,r191,r192,r193,r194 ./$(EXE_OPT) $(MATRIX_SIZE) $(BLOCK_SIZE)
	$(call print_hline)

$(EXE_DEF): $(SRC)
	$(call print_hline)
	$(call print_version)
	$(CXX) $(CXXFLAGS_REPORT) -o $@ $<
	$(call print_hline)

$(EXE_OPT): $(SRC)
	$(call print_hline)
	$(call print_version)
	$(CXX) $(CXXFLAGS_REPORT) $(CXXFLAGS_OPT) -o $@ $<
	$(call print_hline)

$(EXE_OPT_NOVEC): $(SRC)
	$(call print_hline)
	$(call print_version)
	$(CXX) $(CXXFLAGS_REPORT) $(CXXFLAGS_OPT) $(CXXFLAGS_NOVEC) -o $@ $<
	$(call print_hline)

