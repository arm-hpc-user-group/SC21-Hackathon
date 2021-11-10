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

FSRC = $(wildcard *.f90) 
CSRC = profiler_c.c timer_c.c
SRC  = $(FSRC) $(CSRC)

OBJ = $(FSRC:.f90=.o) $(CSRC:.c=.o)

EXE = accelerate_sve.exe

FFLAGS = -g $(FFLAGS_REPORT) $(FFLAGS_OPT)
CFLAGS = -g $(CFLAGS_REPORT) $(CFLAGS_OPT)


.PHONY: all clean run

all: $(EXE)

clean:
	rm -f *.o *.exe *.opt.yaml *.mod *.log

run: $(EXE)
	$(call print_hline)
	$(ARMIE_EXE) -e libmemtrace_sve_$(ARMIE_SVE_VECTOR_BITS).so -i libmemtrace_simple.so -- ./accelerate_sve -nx 500 -ny 500 -its 10
	$(call print_hline)

$(EXE): $(OBJ)
	$(call print_hline)
	$(call print_version)
	$(FC) -o $(EXE) $(FFLAGS) $(OBJ)
	$(call print_hline)

%.o: %.c
	$(CC) -o $@ -c $(CFLAGS) $<

%.o: %.f90
	$(FC) -o $@ -c $(FFLAGS) $<

