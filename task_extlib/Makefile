
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

CC_ARM_OPTS     = -mtune=cortex-a15 -mfloat-abi=hard -mfpu=vfp

CPPFLAGS			 := $(CPPFLAGS) $(CC_DFLAGS) $(CC_ARM_OPTS) -std=c++11
CFLAGS				 := $(CFLAGS) $(CC_DFLAGS)

OA_TC_OPTS			= $(INCLUDE_PATHS) $(CC_DFLAGS)
OA_HC_OPTS			= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) \
									-Wall -Wextra -fopenmp

OA_TL_OPTS			= $(LIBRARY_PATHS) -l:liboam.a
OA_HL_OPTS			= $(LIBRARY_PATHS) -l:liboam.a

OA_SHELL_OPTS		= $(OA_SHELL_FLAGS) \
									--hc="$(OA_HC_OPTS)" --tc="$(OA_TC_OPTS)" \
									--hl="$(OA_HL_OPTS)" --tl="$(OA_TL_OPTS)"

EXE							= task_extlib.bin

HOST_CODE				= task_extlib_main.cpp
HOST_OBJS				= $(patsubst %.cpp, %.o, $(HOST_CODE))

BASE_OAM_LIB		= liboam.a

LIBEXT_TARGET		= libdecoder_target.a

DECODER_LIB			= $(LIBEXT_TARGET)

all: $(EXE)

$(LIBEXT_TARGET):
	$(MAKE) -C decoder/
	@cp decoder/libdecoder_target.a .

$(BASE_OAM_LIB):
	$(MAKE) -C ../base
	@cp ../base/_lib/*.a .


$(EXE): $(HOST_OBJS) $(DECODER_LIB) $(BASE_OAM_LIB)
	$(OA_SHELL) $(OA_SHELL_OPTS) $(HOST_OBJS) $(DECODER_LIB) $(BASE_OAM_LIB) -o $@

%.o: %.cpp
	$(COMPILE.cpp) $(CPPFLAGS) $(INCLUDE_PATHS) -c $<

#	%.o: %.c
#		$(CC) $(CC_OPTS) -c $<

deploy: $(EXE)
	@scp $(EXE) evm:~/

clean:
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log *.a
	@make -C decoder/ clean

help:
	@echo "options:			verbose=1					 verbose build log messages"
	@echo "							buildtype=DEBUG		 build with debug configuration"

