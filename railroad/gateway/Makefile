#
# Makefile for the CAN gateway for BananaPi: https://github.com/GBert/railroad/tree/master/can2udp
# to be run on target	-	C 2018 - 2025 Rainer Müller
#
# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

.PHONY:	all check install clean

# BINS requiring root priviledges
RBINS := s88udp s88test-bpi s88can spitest ir-tester read_lococard
S88UDPOBJ = allwinner.o s88udp-bpi.o

CFLAGS := -DSUNXI_A20 -g -O2 -MD -std=gnu99 -Wall -Wextra
CFLAGS += -fstack-protector-strong -Wformat -Werror=format-security -Wmaybe-uninitialized -pedantic-errors

# modifications for special environment
-include /etc/os-release
ifneq ($(findstring OpenWrt, $(NAME)), )
	DESTDIR = /usr/bin
else
	DESTDIR = /usr/local/bin
	ifneq ($(findstring raspbian, $(ID)), )
		RBINS := s88udp spitest
		S88UDPOBJ = raspi.o s88udp-rpi.o
	else
		DEFS = -DI2C_CARD_PATH=\"/sys/bus/i2c/devices/2-0050/eeprom\"
	endif
endif

BINS := $(RBINS) can2udp can2serial clone-cs2-config clone-cs2-trigger clone-ms2-loco \
        read-cs2-ini gb2-update cs2ping can2lan ms2wifi ms2-loco-list pio-i2c-udp \
        wake-up-c6020 wake-up-links88 lan-schnitte xntcpd 

all:	$(BINS)

install: all
	@echo $(NAME) uses $(DESTDIR)
	chown root:root $(RBINS)
	chmod 4711 $(RBINS)
	-killall -q $(BINS)
	mkdir -p $(DESTDIR)
	cp -u -p $(BINS) $(DESTDIR)
	ln -sf $(DESTDIR)/gb2-update $(DESTDIR)/ms2-update

check:
	cppcheck --enable=all --suppress=missingIncludeSystem . 2> cppcheck.txt 1| cat

lan-schnitte: lan-schnitte.o
	$(CC) $(CFLAGS) -o lan-schnitte lan-schnitte.o

can2udp: can2udp.o
	$(CC) $(CFLAGS) -o can2udp can2udp.o

can2serial: can2serial.o
	$(CC) $(CFLAGS) -o can2serial can2serial.o

cs2ping: cs2ping.o
	$(CC) $(CFLAGS) -o cs2ping cs2ping.o

read-cs2-ini: read-cs2-config.o read-cs2-ini.o
	$(CC) $(CFLAGS) -o read-cs2-ini read-cs2-config.o read-cs2-ini.o

clone-cs2-config: crc-ccitt.o clone-cs2-config.o
	$(CC) $(CFLAGS) -o clone-cs2-config crc-ccitt.o clone-cs2-config.o -lz

clone-cs2-trigger: clone-cs2-trigger.o
	$(CC) $(CFLAGS) -o clone-cs2-trigger clone-cs2-trigger.o -lpthread

clone-ms2-loco: crc-ccitt.o read-cs2-config.o clone-ms2-loco.o
	$(CC) $(CFLAGS) -o clone-ms2-loco crc-ccitt.o read-cs2-config.o clone-ms2-loco.o -lpthread

gb2-update: crc-ccitt.o gb2-update.o
	$(CC) $(CFLAGS) -o gb2-update crc-ccitt.o gb2-update.o

can2lan: ascii-frame.o crc-ccitt.o gio.o read-cs2-config.o can2lan.o
	$(CC) $(CFLAGS) -o can2lan ascii-frame.o crc-ccitt.o gio.o read-cs2-config.o can2lan.o -lz

ms2wifi: ms2wifi.o
	$(CC) $(CFLAGS) -o ms2wifi ms2wifi.o

ms2-loco-list: crc-ccitt.o read-cs2-config.o ms2-loco-list.o
	$(CC) $(CFLAGS) -o ms2-loco-list crc-ccitt.o read-cs2-config.o ms2-loco-list.o

ir-tester: ir-tester.o
	$(CC) $(CFLAGS) -o ir-tester ir-tester.o

pio-i2c-udp: pio-i2c-udp.o
	$(CC) $(CFLAGS) -o pio-i2c-udp pio-i2c-udp.o

read_lococard: read-cs2-config.o read_lococard.o
	$(CC) $(CFLAGS) -o read_lococard read-cs2-config.o read_lococard.o

s88udp: $(S88UDPOBJ)
	$(CC) $(CFLAGS) -o s88udp $(S88UDPOBJ)

s88test-bpi: allwinner.o s88test-bpi.o
	$(CC) $(CFLAGS) -o s88test-bpi allwinner.o s88test-bpi.o

wake-up-links88: wake-up-links88.o
	$(CC) $(CFLAGS) -o wake-up-links88 wake-up-links88.o

wake-up-c6020: wake-up-c6020.o
	$(CC) $(CFLAGS) -o wake-up-c6020 wake-up-c6020.o

xntcpd: xntcpd.o
	$(CC) $(CFLAGS) -o xntcpd xntcpd.o

s88can: s88can.o mcan.o allwinner.o
	$(CC) $(CFLAGS) -o s88can allwinner.o mcan.o s88can.o

spitest: spitest.o
	$(CC) $(CFLAGS) -o spitest spitest.o

clean:
	$(RM) *.o *.d $(BINS) cppcheck.txt

-include *.d

