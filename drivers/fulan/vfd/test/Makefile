#include $(DVD_MAKE)/generic.mak
MDULE_OBJECT=panel
CC=sh4-linux-gcc
LD=sh4-linux-ld

#CFLAGS+= -I./ -I/home/d22cj/workspace/tdt_210210_1455_patched/tdt/cvs/cdk/linux-sh4/include
CFLAGS+= -I./ -I/home/d27lwj/kernel/linux-a25c/linux-sh4/include

CFLAGS+=-I${HOME}/workspace/target/usr/include -g -O0

#vpath	%.c	$(SPARKDRVROOT)/src/pdi/$(MODULE_OBJECT)/test

objects  = input_test.o#test_panel.o

binaries = input_test#test_panel

inst_bin = $(binaries)

.PHONY: all

all: $(binaries)

$(binaries): $(objects)

include Make.rules
