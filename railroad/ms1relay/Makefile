#
# Makefile for the ms1relay project for BananaPi
# to be run on target	-	C 2021 - 2023 Rainer Müller 
#
# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

BINS := ms1relay

## BIN = $@

CFLAGS+=-Wall -MD -pedantic -std=gnu99 -g

# modifications for special environment
-include /etc/os-release
ifneq ($(findstring OpenWrt, $(NAME)), )
	DESTDIR = /usr/bin
else
	DESTDIR = /usr/local/bin
endif

.PHONY:	all check install clean

all: $(BINS)

ms1relay: ms1relay.o ms1rdata.o
	$(CC) $(CFLAGS) -o ms1relay ms1rdata.o ms1relay.o

clean:
	$(RM) *.o *.d $(BINS)

check:
	cppcheck --enable=all .

install: all
	cp -u -p $(BINS) $(DESTDIR)

-include *.d
