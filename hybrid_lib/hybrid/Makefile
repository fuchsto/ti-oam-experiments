
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
CFLAGS				 := $(CFLAGS)   $(INCLUDE_PATHS) $(CC_DFLAGS) -std=c99

OA_TC_OPTS			= -std=c99 $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC
OA_HC_OPTS			= -std=c99 $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) -DOMPACC \
									-Wall -Wextra -fopenmp -ffast-math

OA_TL_OPTS			= $(LIBRARY_PATHS)
OA_HL_OPTS			= $(LIBRARY_PATHS)

OA_SHELL_OPTS		= $(OA_SHELL_FLAGS) \
									--hc="$(OA_HC_OPTS)" --tc="$(OA_TC_OPTS)" \
									--hl="$(OA_HL_OPTS)" --tl="$(OA_TL_OPTS)"

OA_SHELL_LIB_OPTS = $(OA_SHELL_OPTS) --make_lib

LIB							= libhybrid.a

TARGET_SRCS     = target_fun.c
HOST_CODE				= host_fun.c
HOST_OBJS				= $(patsubst %.cpp, %.o, $(HOST_CODE))


BASE_OAM_LIB		= liboam.a

all: $(LIB)

$(LIB): $(HOST_OBJS)
	@echo "=="
	@echo "== hybrid        == OA_SHELL:        LIB: ${@}"
	@echo "=="
	$(OA_SHELL) $(OA_SHELL_LIB_OPTS) $(HOST_OBJS) $(TARGET_SRCS) -o $@

%.o: %.c
	@echo "=="
	@echo "== hybrid        == COMPILE.c:       C object: ${@}"
	@echo "=="
	$(COMPILE.c) $< -o $@

%.o: %.cpp
	@echo "=="
	@echo "== hybrid        == COMPILE.cpp:     CPP object: ${@}"
	@echo "=="
	$(COMPILE.cpp) -std=c++11 $< -o $@

clean:
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log *.a *.pp *.rl *.orig

help:
	@echo "options:			verbose=1					 verbose build log messages"
	@echo "							buildtype=DEBUG		 build with debug configuration"

