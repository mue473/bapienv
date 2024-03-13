#
# Makefile for the Power Event Key Handling
# to be run on target	-	C 2023 - 2024 Rainer Müller
#
# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

BINS = bpipek
CONF = bpipekconf rc.local
DESTDIR = /sbin
CONFDIR = /etc

.PHONY:	all install clean

CFLAGS += -g -O2 -MD -Wall -Wextra -Wmaybe-uninitialized -std=gnu99

all:	$(BINS)

install: all
	-killall -q $(BINS)
	cp -p $(BINS) $(DESTDIR)
	cp -p $(CONF) $(CONFDIR)
	/sbin/bpipek &

clean:
	$(RM) *.d $(BINS)

-include *.d
