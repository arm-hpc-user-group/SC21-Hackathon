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

EXE_NEON   = fmla_neon128.exe
EXE_SVE512 = fmla_sve512.exe
EXE_A64FX  = fmla_a64fx.exe
EXE = $(EXE_NEON) $(EXE_SVE512) $(EXE_A64FX)

SRCS = $(wildcard *.cc)
HDRS = $(wildcard *.h)
OBJS = $(SRCS:.cc=.o)

CXXFLAGS = $(CXXFLAGS_OPT)

.PHONY: all clean run

all: $(EXE)

clean:
	rm -f $(OBJS) $(EXE)

run: $(EXE)
	$(call print_hline)
	./$(EXE_NEON)
	$(call print_hline)
	./$(EXE_SVE512)
	$(call print_hline)
	./$(EXE_A64FX)
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
	perf stat -e r8028,r11,r23,r24,r191,r192,r193,r194 ./$(EXE_NEON)
	perf stat -e r8028,r11,r23,r24,r191,r192,r193,r194 ./$(EXE_SVE512)
	perf stat -e r8028,r11,r23,r24,r191,r192,r193,r194 ./$(EXE_A64FX)

fmla_%.exe: fmla-%.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc Makefile $(HDRS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

