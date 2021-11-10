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


# Total memory: 0.2 GiB
STREAM_ARRAY_SIZE = 10000000
NTIMES = 20

CFLAGS = $(CFLAGS_OPT) -DSTREAM_ARRAY_SIZE=$(STREAM_ARRAY_SIZE) -DNTIMES=$(NTIMES)

SRC = $(wildcard *.c)
HDR = $(wildcard *.h)
OBJ = $(SRC:.c=.o)

EXE = stream_vanilla_$(COMPILER).exe

.PHONY: all clean run

all: $(EXE)

clean:
	rm -f *.o *.exe *.opt.yaml

run: $(EXE)
	./$(EXE)

run-fapp: $(EXE)
	if [[ "$(CC)" != "fcc" ]] ; then \
	  echo "The Fujitsu profiler must be used with the Fujitsu compiler." ;\
	  exit 1 ;\
	fi
	which fapp
	rm -rf profile-fapp
	i=1 ;\
	while [[ $$i -le 17 ]] ; do \
	  fapp -C -d ./profile-fapp/rep$$i -Hevent=pa$$i ./$(EXE) ;\
          ((i++)) ;\
	done
	i=1 ;\
	while [[ $$i -le 17 ]] ; do \
	  fapp -A -d ./profile-fapp/rep$$i -Icpupa,nompi -tcsv -o ./profile-fapp/pa$$i.csv ;\
	  ((i++)) ;\
	done
	cp -v  $(dir $CC)/../misc/cpupa/cpu_pa_report.xlsm ./profile-fapp || \
	  echo "cpu_pa_report.xlsm not found.  Please copy it to ./profile-fapp"
	echo "Profiling complete!  Copy $(PWD)/profile-fapp to a Windows machine for post-processing with Excel"

$(EXE): $(OBJ)
	$(call print_hline)
	$(call print_version)
	$(CC) $(CFLAGS) -o $@ $^
	$(call print_hline)

%.o: %.c $(HDR) Makefile
	$(CC) -c $< $(CFLAGS)
