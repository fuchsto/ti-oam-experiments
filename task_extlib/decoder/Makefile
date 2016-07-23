
CL										= cl6x $(INCLUDE_PATHS)
OA_SHELL							= clacc
CPP										= arm-linux-gnueabihf-g++
CC										= arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES		= *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
												*.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

export LC_ALL=C

ifeq (1,$(verbose))
OA_SHELL_VERBOSE_OPTS = --show_cc_opts -v -k
endif
ifeq (DEBUG, $(buildtype))
OA_SHELL_DEBUG_OPTS   = -d -p -g
CC_DFLAGS							= -DDEBUG -DENABLE_LOGGING
else
CC_DFLAGS				      = -DNDEBUG
endif

OA_SHELL_FLAGS        = $(OA_SHELL_VERBOSE_OPTS) $(OA_SHELL_DEBUG_OPTS)
INCLUDE_PATHS         = -I$(PWD)/include -I../../base/_inc
LIBRARY_PATHS					= -L. -L../../base/_lib

CC_ARM_OPTS						= -march=armv7-a -mtune=cortex-a15 -mfloat-abi=hard -mfpu=neon

CPP_OPTS							= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) -std=c++11
CC_OPTS								= $(INCLUDE_PATHS) $(CC_DFLAGS)

OA_TC_OPTS						= $(INCLUDE_PATHS) $(CC_DFLAGS)
OA_HC_OPTS						= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) -Wall -Wextra -fopenmp

OA_TL_OPTS						= -lm
OA_HL_OPTS						= -lm

OA_SHELL_OPTS					= $(OA_SHELL_FLAGS) \
												--hc="$(OA_HC_OPTS)" \
												--tc="$(OA_TC_OPTS)" \
												--hl="$(OA_HL_OPTS)" \
												--tl="$(OA_TL_OPTS)"

OA_SHELL_LIB_OPTS			= $(OA_SHELL_OPTS) --make_lib

TARGET_EXE						= decoder_t.bin
HOST_EXE							= decoder.bin

TARGET_CODE           = decoder_target.c

BASE_OAM_LIB					= liboam.a

TARGET_LIB            = libdecoder_target.a
LIBS									= $(TARGET_LIB)

all: $(LIBS)


$(BASE_OAM_LIB):
	$(MAKE) -C ../../base
	@cp ../../base/_lib/*.a .


$(TARGET_LIB): $(TARGET_CODE) $(BASE_OAM_LIB)
	$(OA_SHELL) $(OA_SHELL_LIB_OPTS) $< -o $@ $(BASE_OAM_LIB)

%.a: %.c
	$(OA_SHELL) $(OA_SHELL_LIB_OPTS) $< -o lib$@


%.o: %.cpp
	$(CPP) $(CPP_OPTS) -c $<

clean:
	@rm -f $(HOST_EXE) $(TARGET_EXE) $(TARGET_LIB) $(OA_SHELL_TMP_FILES) *.log

