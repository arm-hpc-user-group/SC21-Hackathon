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

include ../../../config.mk


# Total memory: 24GiB
STREAM_ARRAY_SIZE = 1073741824
NTIMES = 20

# Runtime parameters
OMP_ENV = OMP_NUM_THREADS=48 OMP_PROC_BIND=close
NUMACTL = numactl -C 0-47 -l
RUN = $(OMP_ENV) $(NUMACTL)


CFLAGS = $(CFLAGS_OPT) $(CFLAGS_OPENMP) -DSTREAM_ARRAY_SIZE=$(STREAM_ARRAY_SIZE) -DNTIMES=$(NTIMES)

SRC = $(wildcard *.c)
HDR = $(wildcard *.h)
OBJ = $(SRC:.c=.o)

EXE = stream_openmp_$(COMPILER).exe

.PHONY: all clean run

all: $(EXE)

clean:
	rm -f *.o *.exe *.opt.yaml

run: $(EXE)
	$(RUN) ./$(EXE)

run-perf: $(EXE)
	# r11:   CPU_CYCLES: This event counts every cycle
	# r23:   STALL_FRONTEND: every cycle counted by the CPU_CYCLES 
	#        event on that no operation was issued because there 
	#        are no operations available to issue for this PE from 
	#        the frontend.
	# r24:   STALL_BACKEND: every cycle counted by the CPU_CYCLES 
	#        event on that no operation was issued because the 
	#        backend is unable to accept any operations.
	# r184:  every cycle that no instruction was committed because 
	#        the oldest and uncommitted load/store/prefetch operation 
	#        waits for L1D cache, L2 cache and memory access.
	# r191:  1INST_COMMIT: cycles where one instruction is committed.
	# r192:  2INST_COMMIT: cycles where two instructions are committed.
	# r193:  3INST_COMMIT: cycles where three instructions are committed.
	# r194:  4INST_COMMIT: cycles where four instructions are committed.
	$(RUN) perf stat -e r11,r23,r24,r184,r191,r192,r193,r194 ./$(EXE)

$(EXE): $(OBJ)
	$(call print_hline)
	$(call print_version)
	$(CC) $(CFLAGS) -o $@ $^
	$(call print_hline)

%.o: %.c $(HDR) Makefile
	$(CC) -c $< $(CFLAGS)

