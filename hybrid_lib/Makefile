
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
INCLUDE_PATHS		= -I$(PWD)/include  -I../oam/_inc
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

CPPFLAGS			 := $(CPPFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC
CFLAGS				 := $(CFLAGS)   $(INCLUDE_PATHS) $(CC_DFLAGS) \
									-std=c99

OA_TC_OPTS			= $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC
OA_HC_OPTS			= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) -DOMPACC \
									-Wall -Wextra -fopenmp -ffast-math

OA_TL_OPTS			= $(LIBRARY_PATHS)
OA_HL_OPTS			= $(LIBRARY_PATHS)

OA_SHELL_OPTS		= $(OA_SHELL_FLAGS) \
									--hc="$(OA_HC_OPTS)" --tc="$(OA_TC_OPTS)" \
									--hl="$(OA_HL_OPTS)" --tl="$(OA_TL_OPTS)"

EXE							= hybrid_lib.bin

TARGET_SRCS     = hybrid_wrapper.c
HOST_SRCS				= hybrid_main.cpp
HOST_OBJS				= $(patsubst %.cpp, %.o, $(HOST_SRCS))


BASE_OAM_LIB		= liboam.a
HYBRID_LIB			= libhybrid.a

all: $(EXE)

$(BASE_OAM_LIB):
	@make -C ../oam/tioamlinux.oam buildtype=$(buildtype) verbose=$(verbose)
	@cp ../oam/tioamlinux.oam/*.a .

$(HYBRID_LIB):
	@make -C hybrid buildtype=$(buildtype) verbose=$(verbose)
	@cp hybrid/*.a .

$(EXE): $(HOST_OBJS) $(HYBRID_LIB) $(BASE_OAM_LIB)
	@echo "=="
	@echo "== hybrid_lib    == OA_SHELL:        EXE: ${EXE}"
	@echo "=="
	$(OA_SHELL) $(OA_SHELL_OPTS) $(HYBRID_LIB) $(BASE_OAM_LIB) $(HOST_OBJS) $(TARGET_SRCS) -o $@

%.o: %.c
	@echo "=="
	@echo "== hybrid_lib    == COMPILE.c:       C object: ${@}"
	@echo "=="
	$(COMPILE.c) $< -o $@

%.o: %.cpp
	@echo "=="
	@echo "== hybrid_lib    == COMPILE.cpp:     CPP object: ${@}"
	@echo "=="
	$(COMPILE.cpp) -std=c++11 $< -o $@

clean:
	@make -C hybrid clean
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log *.a *.pp *.rl *.orig

help:
	@echo "options:			verbose=1					 verbose build log messages"
	@echo "							buildtype=DEBUG		 build with debug configuration"

