
VPATH =  ../_src/  _src

ifeq (DEBUG, $(buildtype))
CC_DFLAGS							= -DDEBUG -DENABLE_LOGGING
else
CC_DFLAGS							= -DNDEBUG
endif

INCLUDE_PATHS					= -I./_inc \
												-I../_inc \
												-I../portlx/_inc/portlx \
												-I../../portlx/_inc/portlx \
												-I../vs130/_inc \
												-I../../vs130/_inc \
												-I../sysdefs/_inc \
												-I../../sysdefs/_inc \
												-I../basefw/_inc \
												-I../../basefw/_inc \
												-I../common/_inc \
												-I../../common/_inc

CPPFLAGS						 := $(CPPFLAGS) \
												${MV_CPPFLAGS} \
												$(INCLUDE_PATHS) \
												$(CC_DFLAGS) \
												-D_DEBUG \
												-D_LIB \
												-DPCPROTO \
												-DVRDLL_IMPORTS \
												-DANNOTATION \
												-g \
												-fPIC \
												-DARMLINUX \
												-pthread

# <twhf> TODO: Clarify if tilinux targets (without OAM) must be compiled
#              with OpenMP support:
#
# CPPFLAGS					 := $(CPPFLAGS) \
#												-fopenmp \
#												-pthread
CPPFLAGS					 := $(CPPFLAGS) \
												-pthread

CFLAGS							 := $(CFLAGS) \
												-std=c99

CXXFLAGS						 := ${CXXFLAGS} \
											 -std=gnu++11 \
											 -fvisibility-inlines-hidden \
											 -fno-implement-inlines

LDFLAGS							 := ${LDFLAGS} -lrt -lm

SOURCE_DIR						= ../_src
LIB_DIR								= ./

SRCS									= $(SOURCE_DIR)/oam_task.c \
												$(SOURCE_DIR)/oam_comm.c \
												$(SOURCE_DIR)/oam_vsmem.c \
												$(SOURCE_DIR)/oam_task_target.c

OBJS									= oam_task.o \
												oam_comm.o \
												oam_vsmem.o \
												oam_task_target.o

depfiles := $(subst .o,.dep,$(OBJS))

LIB = liboam.a

all: ${LIB}

${LIB}: ${OBJS}
	${AR} ${ARFLAGS} ${LIB} ${OBJS}

clean:
	rm -f ${LIB} ${OBJS} ${depfiles} deps

#####################################
# .dep rules for dependencies
%.dep: %.c
#  Auszuführende Kommandos (eingebaut):
	$(COMPILE.c)   -M -MF $@ $<

%.dep: %.cc
#  Auszuführende Kommandos (eingebaut):
	$(COMPILE.cc)   -M -MF $@  $<

%.dep: %.C
#  Auszuführende Kommandos (eingebaut):
	$(COMPILE.c) -x c  -M -MF $@  $<

%.dep: %.cpp
#  Auszuführende Kommandos (eingebaut):
	$(COMPILE.cpp)  -M -MF $@ $<
#####################################
# force .C to be c not c++
%.o: %.C
#  Auszuführende Kommandos (eingebaut):
	$(COMPILE.c) -x c  $< -o $@
#####################################

deps:	${depfiles}
	cat ${depfiles} > deps

${OBJS}:	Makefile

include deps
