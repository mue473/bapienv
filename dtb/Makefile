#
# Makefile for correction of the sun7i_can DTB overlay
# C 2024 Rainer Müller
#
.PHONY:	default install clean uninstall

default: sun7i-a20-can.dtbo

sun7i-a20-can.dtbo: sun7i-a20-can.dts
	dtc -@ -I dts -O dtb -o sun7i-a20-can.dtbo sun7i-a20-can.dts

install: default
	cp sun7i-a20-can.dtbo /boot/dtb/overlay

clean:
	rm -f sun7i-a20-can.dtbo

uninstall:
	@echo nothing to do for DTB
