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

EXE1 = gather_struct_$(COMPILER).exe
EXE2 = gather_struct_acle_$(COMPILER).exe

EXE = $(EXE1) $(EXE2)

CFLAGS = $(CFLAGS_REPORT) $(CFLAGS_OPT)

.PHONY: all clean run

all: $(EXE)

clean:
	rm -f *.o *.exe *.opt.yaml

run: $(EXE)
	$(call print_hline)
	./$(EXE1)
	$(call print_hline)
	./$(EXE2)
	$(call print_hline)

$(EXE1): gather_struct.c
	$(call print_hline)
	$(call print_version)
	$(CC) $(CFLAGS) -o $@ $<
	$(call print_hline)

$(EXE2): gather_struct_acle.c
	$(call print_hline)
	$(call print_version)
	$(CC) $(CFLAGS) -o $@ $<
	$(call print_hline)

