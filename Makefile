include ../Makefile.param
CROSS_COMPILE=arm-hisiv100-linux-
# CROSS_COMPILE=
# CFLAGS := -Wall -g -O0

CC:=gcc
CC:=$(CROSS_COMPILE)$(CC)
AR:=$(CROSS_COMPILE)$(AR)
RANLIB:=$(CROSS_COMPILE)ranlib
ARFLAGS = rcv
MAKE = make

TARGET = test

all: $(TARGET)

test: test.o motor_ctl.o motor_ctl.o auto_find_extremun.o $(COMM_OBJ)
	$(CC) $(CFLAGS) -lpthread -lm -o $@ $^ $(LIBS) $(AUDIO_LIBA)

.PHONY: clean

clean:
	-rm -f $(TARGET) test.o auto_find_extremun.o
