
ifeq (1,$(verbose))
OA_SHELL_VERBOSE_OPTS = --show_cc_opts -v -k
endif
ifeq (DEBUG, $(buildtype))
OA_SHELL_DEBUG_OPTS		= -d -p -g
CC_DFLAGS							= -DDEBUG -DENABLE_LOGGING
else
CC_DFLAGS							= -DNDEBUG
endif

OA_SHELL_FLAGS	= $(OA_SHELL_VERBOSE_OPTS) $(OA_SHELL_DEBUG_OPTS)
INCLUDE_PATHS		= -I$(PWD)/include  -I../base/_inc
LIBRARY_PATHS   = -L. -L../base/_lib

QMAKE_DEFAULT_INCDIRS := $(QMAKE_DEFAULT_INCDIRS) $(INCLUDE_PATHS)
QMAKE_DEFAULT_LIBDIRS := $(QMAKE_DEFAULT_LIBDIRS) $(LIBRARY_PATHS)

CL									= cl6x $(INCLUDE_PATHS) $(LIBRARY_PATHS)
OA_SHELL						= clacc
CPP									= arm-linux-gnueabihf-g++
CC									= arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES	= *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
											*.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

export LC_ALL=C

# CC_ARM_OPTS     = -mtune=cortex-a15 -mfloat-abi=hard -mfpu=vfp

CPPFLAGS						 := $(CPPFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS)
CFLAGS							 := $(CFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS)

OA_TC_OPTS			= $(INCLUDE_PATHS) $(CC_DFLAGS)
OA_HC_OPTS			= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) \
									-Wall -Wextra -fopenmp -ffast-math

OA_TL_OPTS			= $(LIBRARY_PATHS)
OA_HL_OPTS			= $(LIBRARY_PATHS) -l:libdecoder_host.a -l:liboam.a

OA_SHELL_OPTS		= $(OA_SHELL_FLAGS) \
									--hc="$(OA_HC_OPTS)" --tc="$(OA_TC_OPTS)" \
									--hl="$(OA_HL_OPTS)" --tl="$(OA_TL_OPTS)"

EXE							= extlib_linkage.bin

HOST_CODE				= task_extlib_main.cpp
HOST_OBJS				= $(patsubst %.cpp, %.o, $(HOST_CODE))

BASE_OAM_LIB		= liboam.a

LIBEXT_HOST			= libdecoder_host.a

DECODER_LIB			= $(LIBEXT_HOST)

all: $(EXE)

$(LIBEXT_HOST):
	$(MAKE) -C decoder/
	@cp decoder/libdecoder_host.a .

$(BASE_OAM_LIB):
	$(MAKE) -C ../base
	@cp ../base/_lib/*.a .


$(EXE): $(HOST_OBJS) $(DECODER_LIB) $(BASE_OAM_LIB)
	@echo "=="
	@echo "== task_extlib   == OA_SHELL:        EXE: ${EXE}"
	@echo "=="
	$(OA_SHELL) $(OA_SHELL_OPTS) $(HOST_OBJS) $(DECODER_LIB) $(BASE_OAM_LIB) -o $@

%.o: %.c
	@echo "=="
	@echo "== task_extlib   == COMPILE.c:       C object: ${@}"
	@echo "=="
	$(COMPILE.c) $< -o $@

%.o: %.cpp
	@echo "=="
	@echo "== task_extlib   == COMPILE.cpp:     CPP object: ${@}"
	@echo "=="
	$(COMPILE.cpp) -std=c++11 $< -o $@

deploy: $(EXE)
	@scp $(EXE) evm:~/

clean:
	$(MAKE) -C ../base clean
	$(MAKE) -C decoder/ clean
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log *.a

help:
	@echo "options:			verbose=1					 verbose build log messages"
	@echo "							buildtype=DEBUG		 build with debug configuration"

