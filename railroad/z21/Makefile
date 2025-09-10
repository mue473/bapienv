#
# Makefile for the Z21-Emulation for BananaPi: https://github.com/GBert/railroad/tree/master/z21
# to be run on target	-	C 2023 - 2025 Rainer Müller
#
# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

BINS := cs2toz21 z21emu read-cs2-config_web getcs2ip
		
.PHONY:	all check install clean

all:	$(BINS)

CFLAGS+=-g -O2 -MD -fstack-protector-strong -Wformat -Werror=format-security -Wall -Wextra -Wmaybe-uninitialized -std=gnu99 -pedantic-errors
 
# modifications for special environment
-include /etc/os-release
ifneq ($(findstring OpenWrt, $(NAME)), )
	DESTDIR=/usr/bin
else
	DESTDIR=/usr/local/bin
endif

install: all
	mkdir -p $(DESTDIR)
	cp -u -p $(BINS) $(DESTDIR)

check:
	cppcheck --enable=all --suppress=missingIncludeSystem . 2> cppcheck.txt 1| cat

Z21EMU_OBJS = net.o cs2_net.o cs2-data-functions.o measurement.o read-cs2-config.o subscribe.o utils.o xpn_tty.o z21emu.o

read-cs2-config_web: geturl.o read-cs2-config.o read-cs2-config_web.o
	$(CC) $(CFLAGS) -o read-cs2-config_web geturl.o read-cs2-config.o read-cs2-config_web.o -lcurl

cs2toz21: net.o cs2_net.o geturl.o read-cs2-config.o utils.o cs2toz21.o
	$(CC) $(CFLAGS) -o cs2toz21 net.o cs2_net.o geturl.o read-cs2-config.o utils.o cs2toz21.o -lsqlite3 -lcurl -luuid

getcs2ip: cs2_net.o net.o utils.o getcs2ip.o
	$(CC) $(CFLAGS) -o getcs2ip cs2_net.o net.o utils.o getcs2ip.o

z21emu: ${Z21EMU_OBJS}
	$(CC) $(CFLAGS) -o z21emu ${Z21EMU_OBJS} -lpthread

clean:
	$(RM) *.o *.d $(BINS) cppcheck.txt

-include *.d
