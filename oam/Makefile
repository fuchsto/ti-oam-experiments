
CL										= cl6x $(INCLUDE_PATHS)
OA_SHELL							= clacc
CPP										= arm-linux-gnueabihf-g++
CC										= arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES		= *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
												*.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

export LC_ALL=C

UNAME_M :=$(shell uname -m)
ifneq (,$(findstring x86, $(UNAME_M)))
 ifeq ($(TARGET_ROOTDIR),)
 $(call error,ERROR - TARGET_ROOTDIR must be set for cross compiling)
 endif
endif

ifeq ($(TI_OCL_CGT_INSTALL),)
  TI_OCL_CGT_INSTALL = $(TARGET_ROOTDIR)/usr/share/ti/cgt-c6x
endif


ifeq (1,$(verbose))
OA_SHELL_VERBOSE_OPTS = --show_cc_opts -v -k
endif
ifeq (DEBUG, $(buildtype))
OA_SHELL_DEBUG_OPTS		= -d -p -g
CC_DFLAGS							= -DDEBUG -DENABLE_LOGGING
else
CC_DFLAGS							= -DNDEBUG
endif

OA_SHELL_FLAGS				= $(OA_SHELL_VERBOSE_OPTS) $(OA_SHELL_DEBUG_OPTS)
INCLUDE_PATHS					= -I./_inc \
												-I../_inc \
												-I../vs130/_inc \
												-I../../vs130/_inc \
												-I../sysdefs/_inc \
												-I../../sysdefs/_inc \
												-I../basefw/_inc \
												-I../../basefw/_inc

CPPFLAGS						 := $(CPPFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS)
CFLAGS							 := $(CFLAGS) $(INCLUDE_PATHS) $(CC_DFLAGS)

OA_TC_OPTS						= $(INCLUDE_PATHS) $(CC_DFLAGS)
OA_HC_OPTS						= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) \
												-Wall -Wextra -Wno-empty-body -Wno-unused-parameter \
												-fopenmp

OA_TL_OPTS						= -lm -lrt
OA_HL_OPTS						= -lm -lrt

OA_SHELL_OPTS					= $(OA_SHELL_FLAGS) \
												--hc="$(OA_HC_OPTS)" \
												--tc="$(OA_TC_OPTS)" \
												--hl="$(OA_HL_OPTS)" \
												--tl="$(OA_TL_OPTS)"

OA_SHELL_LIB_OPTS			= $(OA_SHELL_OPTS) --make_lib

SOURCE_DIR						= ./_src
LIB_DIR								= ./
BUILD_DIR							= .

HOST_CODE							= $(SOURCE_DIR)/oam_task.c

TARGET_CODE						=	$(SOURCE_DIR)/oam_comm.c \
												$(SOURCE_DIR)/oam_vsmem.c \
												$(SOURCE_DIR)/oam_task_target.c

HOST_OBJS							= $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(HOST_CODE))

LIB										= ${LIB_DIR}/liboam.a

all: $(LIB_DIR) $(BUILD_DIR) $(LIB)

$(LIB): $(HOST_OBJS) $(TARGET_CODE)
	@echo "=="
	@echo "== liboam        == OA_SHELL:        LIB: ${@}"
	@echo "=="
	$(OA_SHELL) $(OA_SHELL_LIB_OPTS) $(HOST_OBJS) $(TARGET_CODE) -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@echo "=="
	@echo "== liboam        == COMPILE.cpp:     CPP object: ${@}"
	@echo "=="
	$(COMPILE.cpp) -std=c++11 $< -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@echo "=="
	@echo "== liboam        == COMPILE.c:       C object: ${@}"
	@echo "=="
	$(COMPILE.c) $< -o $@

.PHONY: all $(BUILD_DIR) clean

$(BUILD_DIR):
	@mkdir -p $@

$(LIB_DIR):
	@mkdir -p $@

clean:
	@rm -f $(BUILD_DIR)/*.o
	@rmdir $(BUILD_DIR) >/dev/null 2>&1 || true
	@rm -f $(LIB_DIR)/*.o
	@rmdir $(LIB_DIR) >/dev/null 2>&1 || true
	@rm -f $(LIB) $(OA_SHELL_TMP_FILES) *.log

