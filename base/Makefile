
CL										= cl6x $(INCLUDE_PATHS)
OA_SHELL							= clacc
CPP										= arm-linux-gnueabihf-g++
CC										= arm-linux-gnueabihf-gcc
OA_SHELL_TMP_FILES		= *.out __TI_CLACC_KERNEL.c *.cl *.asm *.dsp_h \
												*.bc *.objc *.if *.map *.opt *.int.c *.o *.obj

# temporary workaround for locale bug
# LC_ALL?=C
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
INCLUDE_PATHS         = -I$(PWD)/_inc -I../_inc

CC_ARM_OPTS						= -march=armv7-a -mtune=cortex-a15 -mfloat-abi=hard -mfpu=neon

CPP_OPTS							= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) -std=c++11
CC_OPTS								= $(INCLUDE_PATHS) $(CC_DFLAGS)

OA_TC_OPTS						= $(INCLUDE_PATHS) $(CC_DFLAGS)
OA_HC_OPTS						= $(INCLUDE_PATHS) $(CC_DFLAGS) $(CC_ARM_OPTS) \
												-Wall -Wextra -Wno-empty-body -Wno-unused-parameter \
												-fopenmp

OA_TL_OPTS						= -lm
OA_HL_OPTS						= -lm

OA_SHELL_OPTS					= $(OA_SHELL_FLAGS) \
												--hc="$(OA_HC_OPTS)" \
												--tc="$(OA_TC_OPTS)" \
												--hl="$(OA_HL_OPTS)" \
												--tl="$(OA_TL_OPTS)"

OA_SHELL_LIB_OPTS			= $(OA_SHELL_OPTS) --make_lib

SOURCE_DIR            = _src
LIB_DIR               = _lib
BUILD_DIR             = build

HOST_CODE             = $(SOURCE_DIR)/oam_task.c

TARGET_CODE           =	$(SOURCE_DIR)/oam_comm.c \
												$(SOURCE_DIR)/oam_vsmem.c \
												$(SOURCE_DIR)/oam_task_target.c

HOST_OBJS							= $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(HOST_CODE))

# TARGET_LIB          = liboam_target.a
# HOST_LIB	          = liboam_host.a
# LIBS								= $(TARGET_LIB) $(HOST_LIB)
LIB										= $(LIB_DIR)/liboam.a

all: $(LIB_DIR) $(BUILD_DIR) $(LIB)

$(LIB): $(HOST_OBJS) $(TARGET_CODE)
	$(OA_SHELL) $(OA_SHELL_LIB_OPTS) $(HOST_OBJS) $(TARGET_CODE) -o $@

#	$(TARGET_LIB): $(TARGET_CODE)
#		$(OA_SHELL) $(OA_SHELL_LIB_OPTS) $< -o $@

#	$(HOST_LIB): $(HOST_OBJS)
#		ar rcs $@ $^

#	%.a: %.c
#		$(OA_SHELL) $(OA_SHELL_LIB_OPTS) $< -o lib$@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CPP) $(CPP_OPTS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CC_OPTS) -c $< -o $@

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

