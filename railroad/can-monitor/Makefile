#
# Makefile for the CAN-Monitor for BananaPi: https://github.com/GBert/railroad/tree/master/can-monitor
# to be run on target	-	C 2023 Rainer Müller
#
# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

BINS := can-monitor
		
.PHONY:	all check install clean

all:	$(BINS)

CFLAGS+=-g -O2 -MD -fstack-protector-strong -Wformat -Werror=format-security -Wall -Wextra -Wmaybe-uninitialized -std=gnu99 -pedantic-errors
 
# modifications for special environment
-include /etc/os-release
ifneq ($(findstring OpenWrt, $(NAME)), )
	DESTDIR=/usr/bin
else
	DESTDIR=/usr/local/bin
	SETCAP=setcap CAP_NET_RAW,CAP_NET_ADMIN+eip $(DESTDIR)/$(BINS)
endif

install: all
	@echo $(NAME) uses $(DESTDIR)
	mkdir -p $(DESTDIR)
	cp -u -p $(BINS) $(DESTDIR)
	$(SETCAP)

check:
	cppcheck --enable=all . 2> cppcheck.txt

OBJS = crc-ccitt.o lib.o decoder-can-cdb.o decoder-can-cs1.o decoder-can-cs2.o decoder-can-maecan.o decoder-z21.o tools.o can-monitor.o

can-monitor: ${OBJS}
	$(CC) $(CFLAGS) -o can-monitor ${OBJS} -lpcap -lz

clean:
	$(RM) *.o *.d $(BINS)

-include *.d
