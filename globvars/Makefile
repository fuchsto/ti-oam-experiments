# include ../make.inc


export MV460_BASE=".."


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
INCLUDE_PATHS   = -I$(PWD)/include -I../oam/_inc/
LIBRARY_PATHS   = -L$(PWD)../oam/_lib

CL                    = cl6x $(INCLUDE_PATHS)
OA_SHELL              = clacc
CPP		                = arm-linux-gnueabihf-g++
CC		                = arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES    = *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
                        *.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

export LC_ALL=C

CC_ARM_OPTS     = -march=armv7-a -mtune=cortex-a15 -mfloat-abi=hard -mfpu=vfp

CPP_OPTS        = $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) -std=c++11 -fopenmp
CC_OPTS         = $(INCLUDE_PATHS) $(CC_DFLAGS) -fopenmp

OA_TC_OPTS      = $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC
OA_HC_OPTS      = $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC $(CC_ARM_OPTS) \
									-Wall -Wextra -fopenmp -ffast-math

OA_TL_OPTS			= $(LIBRARY_PATHS)
OA_HL_OPTS			= $(LIBRARY_PATHS)

OA_SHELL_OPTS   = $(OA_SHELL_FLAGS) \
									--hc="$(OA_HC_OPTS)" \
									--tc="$(OA_TC_OPTS)" \
									--hl="$(OA_HL_OPTS)" \
									--tl="$(OA_TL_OPTS)"

# LIB_OWNER				= lib_owner.a
LIB_CLIENT			= lib_client_target.t.a
LIB_OWNER				= lib_owner_target.a

LIB_OWNER_SRC		= lib_owner_target.c
LIB_CLIENT_SRC	= lib_client_target.c

HOST_CODE				= globvars_main.cpp
# TARGET_CODE			= globvars_target.c

EXE							= globvars.bin

OBJS						= $(patsubst %.cpp, %.o, $(HOST_CODE))


all: $(EXE) $(LIB_OWNER) $(LIB_CLIENT)

$(BASE_OAM_LIB):
	$(MAKE) -C ../oam
	@cp ../oam/_lib/*.a .

$(EXE): $(OBJS) $(LIB_OWNER) $(LIB_CLIENT)
	$(OA_SHELL) $(OA_SHELL_OPTS) \
							$(OBJS) \
							$(LIB_OWNER) $(LIB_CLIENT) -o $@

#							$(TARGET_CODE) -o $@


%.t.a: %.c
	$(OA_SHELL) $(OA_SHELL_OPTS) --make_lib $< -o $@

%.a: %.o
	${AR} ${ARFLAGS} $@ $<

%.o: %.cpp
	$(CPP) $(CPP_OPTS) -c $<

%.o: %.c
	$(CC) $(CC_OPTS) -c $<

clean:
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log

help:
	@echo "options:      verbose=1           verbose build log messages"
	@echo "              buildtype=DEBUG     build with debug configuration"

