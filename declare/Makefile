# include ../make.inc

ifeq (1,$(verbose))
OA_SHELL_VERBOSE_OPTS = --show_cc_opts -v -k
endif
ifeq (DEBUG, $(buildtype))
OA_SHELL_DEBUG_OPTS   = -d -p -g
CC_DFLAGS             = -DDEBUG -DENABLE_LOGGING
else
CC_DFLAGS             = -DNDEBUG
endif

OA_SHELL_FLAGS  = $(OA_SHELL_VERBOSE_OPTS) $(OA_SHELL_DEBUG_OPTS)
INCLUDE_PATHS   = -I$(PWD)/include -I../base/_inc
LIBRARY_PATHS   = -L$(PWD)../base/_lib -L$(PWD)

CL                    = cl6x $(INCLUDE_PATHS)
OA_SHELL              = clacc
CPP		                = arm-linux-gnueabihf-g++
CC		                = arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES    = *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
                        *.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

export LC_ALL=C

# CC_ARM_OPTS     = -mtune=cortex-a15 -mfloat-abi=hard -mfpu=vfp

CPPFLAGS						 := $(CPPFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS)
CFLAGS							 := $(CFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS)

OA_TC_OPTS      = $(INCLUDE_PATHS) $(CC_DFLAGS)
OA_HC_OPTS      = $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) \
									-Wall -Wextra -fopenmp -ffast-math

OA_TL_OPTS			= $(LIBRARY_PATHS) -l:liboam.a
OA_HL_OPTS			= $(LIBRARY_PATHS) -l:liboam.a

OA_SHELL_OPTS   = $(OA_SHELL_FLAGS) --hc="$(OA_HC_OPTS)" --tc="$(OA_TC_OPTS)" --hl="$(OA_HL_OPTS)" --tl="$(OA_TL_OPTS)"

BASE_OAM_LIB		= liboam.a

EXE							= declare.bin
HOST_CODE				= declare_main.cpp
TARGET_CODE			= declare_target.c ext_interface.c nested_interface.c
OBJS						= $(patsubst %.cpp, %.o, $(HOST_CODE))

all: $(EXE)

$(EXE): $(OBJS) $(BASE_OAM_LIB)
	@echo "=="
	@echo "== declare    == OA_SHELL:        EXE: ${@}"
	@echo "=="
	$(OA_SHELL) $(OA_SHELL_OPTS) $(OBJS) $(BASE_OAM_LIB) $(TARGET_CODE) -o $@

$(BASE_OAM_LIB):
	$(MAKE) -C ../base
	@cp ../base/_lib/*.a .

%.o: %.c
	@echo "=="
	@echo "== declare    == COMPILE.c:       C object: ${@}"
	@echo "=="
	$(COMPILE.c) $< -o $@

%.o: %.cpp
	@echo "=="
	@echo "== declare    == COMPILE.cpp:     CPP object: ${@}"
	@echo "=="
	$(COMPILE.cpp) -std=c++11 $< -o $@

deploy: $(EXE)
	scp $(EXE) evm:~/

clean:
	$(MAKE) -C ../base clean
	@rm -f $(EXE) $(BASE_OAM_LIB) $(OA_SHELL_TMP_FILES) *.log

help:
	@echo "options:      verbose=1           verbose build log messages"
	@echo "              buildtype=DEBUG     build with debug configuration"

