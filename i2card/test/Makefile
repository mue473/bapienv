#	C 2025 Rainer Müller
#
# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

.PHONY:	all check install clean

CFLAGS := -g -O2 -MD -std=gnu99 -Wall -Wextra -pedantic-errors

BINS := i2ciodemo i2c256

all:	$(BINS)

install: all
	chown root:root $(BINS)
	chmod 4711 $(BINS)

check:
	cppcheck --enable=all . 2> cppcheck.txt 1| cat

i2ciodemo: i2ciodemo.o
	$(CC) $(CFLAGS) -o i2ciodemo i2ciodemo.o

i2c256: i2c256.o
	$(CC) $(CFLAGS) -o i2c256 i2c256.o

clean:
	$(RM) *.o *.d $(BINS)

-include *.d
