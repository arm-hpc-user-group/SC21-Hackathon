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

.PHONY: all clean run

ifeq (arm,$(COMPILER))
CLOVERLEAF_COMPILER = ARM
else
ifeq (gnu,$(COMPILER))
CLOVERLEAF_COMPILER = GNU
else
ifeq (cray,$(COMPILER))
CLOVERLEAF_COMPILER = CRAY
else
$(error This example does not support COMPILER=$(COMPILER))
endif
endif
endif

CLOVERLEAF_DIR = ./CloverLeaf_ref

EXE = $(CLOVERLEAF_DIR)/clover_leaf

all: $(EXE)

$(EXE): 
	$(MAKE) -C $(CLOVERLEAF_DIR) COMPILER=$(CLOVERLEAF_COMPILER)

clean:
	$(MAKE) -C $(CLOVERLEAF_DIR) clean

run: $(EXE)
	./$^
	OMP_NUM_THREADS=2 OMP_PROC_BIND=spread mpirun -np 4 $^

