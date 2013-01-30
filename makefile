#===============================================================================
# User Options
#===============================================================================

COMPILER = gnu
OPTIMIZE = yes
DEBUG    = no
PROFILE  = no
PAPI     = no
VEC_INFO = no

#===============================================================================
# Program name & source code list
#===============================================================================

program = XSBench

source = \
Main.c \
CalculateXS.c \
GridInit.c \
XSutils.c \
Materials.c

#===============================================================================
# Sets Flags
#===============================================================================

# Regular gcc Compiler
ifeq ($(COMPILER),gnu)
  GCC = gcc
endif

# BG/Q gcc Cross-Compiler
ifeq ($(MACHINE),bluegene)
  GCC = /bgsys/drivers/toolchain/V1R1M2/gnu-linux/bin/powerpc64-bgq-linux-gcc
endif

# Standard Flags
GCC_S_FLAGS := -fopenmp -Wall -std=c99
GCC_O_FLAGS =
LDFLAGS = -lm

# Debug Flags
ifeq ($(DEBUG),yes)
  GCC_O_FLAGS += -g
  LDFLAGS  += -g
endif

# Profiling Flags
ifeq ($(PROFILE),yes)
  GCC_O_FLAGS += -pg
  LDFLAGS  += -pg
endif

# Optimization Flags
ifeq ($(OPTIMIZE),yes)
  GCC_O_FLAGS += -O3
endif

# Compiler Vectorization (needs -O3 flag) information
ifeq ($(VEC_INFO),yes)
  GCC_O_FLAGS += -ftree-vectorizer-verbose=6
endif

# PAPI source 
ifeq ($(PAPI),yes)
  source += papi.c
endif

#===============================================================================
# Targets to Build
#===============================================================================

$(program): $(source) do_flops.o XSbench_header.h
	$(GCC) $(GCC_S_FLAGS) $(GCC_O_FLAGS) do_flops.o $(source) -o $@ $(LDFLAGS)
do_flops.o: do_flops.c
	$(GCC) $(GCC_S_FLAGS) -c do_flops.c

clean:
	rm -rf XSBench XSBench.dSYM do_flops.o
edit:
	vim -p $(source) do_flops.c papi.c XSbench_header.h
run:
	./XSBench
bgqrun:
	qsub -t 10 -n 1 -O test XSBench
