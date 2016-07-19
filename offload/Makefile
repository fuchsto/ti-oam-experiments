# include ../make.inc

ifeq (1,$(verbose))
OA_SHELL_VERBOSE_OPTS = --show_cc_opts -v
endif
ifeq (DEBUG, $(buildtype))
OA_SHELL_DEBUG_OPTS   = -d -p -g -k
# CC_DFLAGS             = -DDEBUG -DENABLE_LOGGING
else
# CC_DFLAGS             = -DNDEBUG
endif

OA_SHELL_FLAGS  = $(OA_SHELL_VERBOSE_OPTS) $(OA_SHELL_DEBUG_OPTS)
INCLUDE_PATHS   = -I$(PWD)/include

CL                    = cl6x $(INCLUDE_PATHS)
OA_SHELL              = clacc
CPP		                = arm-linux-gnueabihf-g++
CC		                = arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES    = *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
                        *.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

# temporary workaround for locale bug
LC_ALL?=C
export LC_ALL

CC_ARM_OPTS     = -march=armv7-a -mtune=cortex-a15 -mfloat-abi=hard -mfpu=vfp

CPP_OPTS        = $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS)
CC_OPTS         = $(INCLUDE_PATHS) $(CC_DFLAGS)

OA_TC_OPTS      = $(INCLUDE_PATHS) $(CC_DFLAGS)
OA_HC_OPTS      = $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) -Wall -Wextra -fopenmp -ffast-math

OA_TL_OPTS      = -lm
OA_HL_OPTS      = -lm

OA_SHELL_OPTS   = $(OA_SHELL_FLAGS) --hc="$(OA_HC_OPTS)" --tc="$(OA_TC_OPTS)" --hl="$(OA_HL_OPTS)" --tl="$(OA_TL_OPTS)"

EXE         = offload.bin
HOST_CODE   = offload_main.cpp
TARGET_CODE = offload_target.c
OBJS        = $(patsubst %.cpp, %.o, $(HOST_CODE))

$(EXE): $(OBJS)
	$(OA_SHELL) $(OA_SHELL_OPTS) $(OBJS) $(TARGET_CODE) -o $@

%.o: %.cpp
	$(CPP) $(CPP_OPTS) -c $<

%.o: %.c
	$(CC) $(CC_OPTS) -c $<

deploy: $(EXE)
	@scp $(EXE) evm:~/run/offload/

clean:
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log

help:
	@echo "options:      verbose=1           verbose build log messages"
	@echo "              buildtype=DEBUG     build with debug configuration"

