include ../make.inc

ifeq (1,$(verbose))
OA_SHELL_VERBOSE_OPTS = --show_cc_opts -v
endif
ifeq (DEBUG, $(buildtype))
OA_SHELL_DEBUG_OPTS   = -d -p -g
CC_DFLAGS             = -DDEBUG -DENABLE_LOGGING
else
CC_DFLAGS             = -DNDEBUG
endif

OA_SHELL_FLAGS  = $(OA_SHELL_VERBOSE_OPTS) $(OA_SHELL_DEBUG_OPTS)
INCLUDE_FLAGS   = -I. -I./include

# =========================================================================
# Using FPU type 'vfp' as 'neon' does not fully implement IEEE 754
# (no denorms, therefore loss of precision).
#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# !! Check the architecture's CPU features via                           !!
# !!   $ cat /proc/cpuinfo                                               !!
# !! and update compiler flags accordingly.                              !!
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
# Documentation of ARM-specific flags provided by GCC:
#
#   https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
# =========================================================================
# CC_ARM_OPTS     = -march=armv7-a -mtune=cortex-a15 \
#                   -mfloat-abi=hard -mfpu=vfp \

CPP_OPTS        = $(INCLUDE_FLAGS) -O3 $(CC_DFLAGS) $(CC_ARM_OPTS)

OA_TC_OPTS      = -O3 $(INCLUDE_FLAGS) $(CC_DFLAGS)
OA_HC_OPTS      = -O3 $(INCLUDE_FLAGS) $(CC_DFLAGS) \
                  -Wall -Wextra \
                  -fopenmp \
                  -ffast-math -lm \
                  $(CC_ARM_OPTS)

OA_SHELL_OPTS   = -k $(OA_SHELL_FLAGS) \
                  --hc="$(OA_HC_OPTS)" \
                  --tc="$(OA_TC_OPTS)"

EXE         = dsp_img_filter.bin
HOST_CODE   = dsp_img_filter_main.cpp pgm.cpp
TARGET_CODE = dsp_img_filter_target.c
OBJS        = $(patsubst %.cpp, %.o, $(HOST_CODE))

$(EXE): $(OBJS)
	$(OA_SHELL) $(OA_SHELL_OPTS) $(OBJS) $(TARGET_CODE) -o $@

%.o: %.cpp
	$(CPP) $(CPP_OPTS) -c $<

clean:
	@rm -f $(EXE) $(OA_SHELL_TMP_FILES) *.log

help:
	@echo "options:      verbose=1           verbose build log messages"
	@echo "              buildtype=DEBUG     build with debug configuration"

