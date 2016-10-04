# include ../make.inc


export MV460_BASE=$(PWD)/..


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
LIBRARY_PATHS   = -L$(PWD)/../oam/

CL                    = cl6x
OA_SHELL              = clacc
CPP		                = arm-linux-gnueabihf-g++
CC		                = arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES    = *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
                        *.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

export LC_ALL=C

CC_ARM_OPTS				= -march=armv7-a -mtune=cortex-a15 -mfloat-abi=hard -mfpu=vfp

CPP_OPTS					= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) -std=c++11 -fopenmp
CC_OPTS						= $(INCLUDE_PATHS) $(CC_DFLAGS) -fopenmp

OA_TC_OPTS				= $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC
OA_HC_OPTS				= $(INCLUDE_PATHS) $(CC_DFLAGS) -DOMPACC $(CC_ARM_OPTS) \
										-Wall -Wextra -fopenmp -ffast-math

OA_TL_OPTS				= $(LIBRARY_PATHS)
OA_HL_OPTS				= $(LIBRARY_PATHS)

OA_SHELL_OPTS			= $(OA_SHELL_FLAGS) \
										--hc="$(OA_HC_OPTS)" \
										--tc="$(OA_TC_OPTS)" \
										--hl="$(OA_HL_OPTS)" \
										--tl="$(OA_TL_OPTS)"

BASE_OAM_LIB			= liboam.a

# LIB_OWNER				= lib_owner.a
LIB_CLIENT				= lib_client_target.t.a
LIB_OWNER_T				= lib_owner_target.t.a
LIB_OWNER_H				= lib_owner_host.a

LIB_OWNER_H_SRC		= lib_owner_host.c
LIB_OWNER_T_SRC		= lib_owner_target.c
LIB_CLIENT_T_SRC	= lib_client_target.c

HOST_CODE					= globvars_main.cpp

EXE								= globvars.bin

OBJS							= $(patsubst %.cpp, %.o, $(HOST_CODE))


all: $(EXE) $(LIB_OWNER) $(LIB_CLIENT)

$(BASE_OAM_LIB):
	$(MAKE) -C ../oam/tioamlinux.oam
	@cp ../oam/tioamlinux.oam/*.a .

$(EXE): $(OBJS) $(LIB_OWNER_H) $(LIB_OWNER_T) $(LIB_CLIENT) $(BASE_OAM_LIB)
	$(OA_SHELL) $(OA_SHELL_OPTS) \
							$(OBJS) \
							$(LIB_OWNER_H) $(LIB_OWNER_T) \
							$(LIB_CLIENT) \
							$(BASE_OAM_LIB) \
							-o $@

#							$(TARGET_CODE) -o $@


$(LIB_CLIENT): $(BASE_OAM_LIB)
	$(OA_SHELL) $(OA_SHELL_OPTS) --make_lib $(LIB_CLIENT_T_SRC) \
							-o $@ $(BASE_OAM_LIB)

$(LIB_OWNER_T): $(BASE_OAM_LIB)
	$(OA_SHELL) $(OA_SHELL_OPTS) --make_lib $(LIB_OWNER_T_SRC) \
							-o $@ $(BASE_OAM_LIB)

%.a: %.o
	${AR} ${ARFLAGS} $@ $<

%.o: %.cpp
	$(CPP) $(CPP_OPTS) -c $<

%.o: %.c
	$(CC) $(CC_OPTS) -c $<

clean:
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log *.rl *.pp *.a *.c.orig *.int.c
	$(MAKE) -C ../oam/tioamlinux.oam clean

help:
	@echo "options:      verbose=1           verbose build log messages"
	@echo "              buildtype=DEBUG     build with debug configuration"

