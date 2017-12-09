# include ../make.inc

ifeq (1,$(verbose))
OA_SHELL_VERBOSE_OPTS = --show_cc_opts -v -k
endif
ifeq (DEBUG, $(buildtype))
# OA_SHELL_DEBUG_OPTS   = -d -p -g
OA_SHELL_DEBUG_OPTS   = -g
CC_DFLAGS             = -DDEBUG -DMV_LOG__ENABLED -DMV_LOG__OAM__VSMEM # -DMV_LOG__OAM__COMM
else
CC_DFLAGS             = -DNDEBUG
endif

OA_SHELL_FLAGS  = $(OA_SHELL_VERBOSE_OPTS) $(OA_SHELL_DEBUG_OPTS)
INCLUDE_PATHS   = -I$(PWD)/include -I../oam/_inc
LIBRARY_PATHS   = -L$(PWD)../oam/_lib -L$(PWD)

CL                    = cl6x $(INCLUDE_PATHS)
OA_SHELL              = clacc
CPP		                = arm-linux-gnueabihf-g++
CC		                = arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES    = *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
                        *.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

export LC_ALL=C

# CC_ARM_OPTS     = -mtune=cortex-a15 -mfloat-abi=hard -mfpu=vfp

CPPFLAGS						 := $(CPPFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC
CFLAGS							 := $(CFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS)

OA_TC_OPTS      = $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC
OA_HC_OPTS      = $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC $(CC_ARM_OPTS) \
									-Wall -Wextra -fopenmp -ffast-math

OA_TL_OPTS			= -lm -lrt $(LIBRARY_PATHS) # -l:liboam.a
OA_HL_OPTS			= -lm -lrt $(LIBRARY_PATHS) # -l:liboam.a

OA_SHELL_OPTS   = $(OA_SHELL_FLAGS) --hc="$(OA_HC_OPTS)" --tc="$(OA_TC_OPTS)" --hl="$(OA_HL_OPTS)" --tl="$(OA_TL_OPTS)"

BASE_OAM_LIB		= liboam.a

TASK_A_OAM_LIB  = task_a.a
TASK_B_OAM_LIB  = task_b.a

TASK_LIBS       = $(TASK_A_OAM_LIB) $(TASK_B_OAM_LIB)

EXE							= concurrent.bin
HOST_CODE				= concurrent_main.cpp


TARGET_CODE			= concurrent_target.c
OBJS						= $(patsubst %.cpp, %.o, $(HOST_CODE))

all: $(EXE)

$(EXE): $(OBJS) $(BASE_OAM_LIB) $(TASK_LIBS)
	@echo "=="
	@echo "== comm_async    == OA_SHELL:        EXE: ${@}"
	@echo "=="
	$(OA_SHELL) $(OA_SHELL_OPTS) $(OBJS) \
		          $(BASE_OAM_LIB)          \
							$(TASK_LIBS)             \
							$(TARGET_CODE) -o $@

$(TASK_A_OAM_LIB):
	$(MAKE) -C ./task_a
	@cp ./task_a/*.a .

$(TASK_B_OAM_LIB):
	$(MAKE) -C ./task_a
	@cp ./task_a/*.a .

$(BASE_OAM_LIB):
	$(MAKE) -C ../oam/tioamlinux.oam
	@cp ../oam/tioamlinux.oam/*.a .

%.o: %.c
	@echo "=="
	@echo "== comm_async    == COMPILE.c:       C object: ${@}"
	@echo "=="
	$(COMPILE.c) $< -o $@

%.o: %.cpp
	@echo "=="
	@echo "== comm_async    == COMPILE.cpp:     CPP object: ${@}"
	@echo "=="
	$(COMPILE.cpp) -std=c++11 $< -o $@

deploy: $(EXE)
	scp $(EXE) evm:~/

clean:
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log *.rl *.pp *.a *.c.orig *.int.c
	$(MAKE) -C ../oam/tioamlinux.oam clean
	$(MAKE) -C ./task_a clean
	$(MAKE) -C ./task_b clean

help:
	@echo "options:      verbose=1           verbose build log messages"
	@echo "              buildtype=DEBUG     build with debug configuration"

