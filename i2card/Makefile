#
# Makefile for the i2card driver for BananaPi
# C 2023 - 2024 Rainer Müller 
#
.PHONY:	default install clean uninstall

# building specific or the current running version
ifndef KVERSION
KVERSION	:= $(shell uname -r)
endif
MODBASE := /lib/modules/$(KVERSION)

ifneq ($(KERNELRELEASE),)
obj-m := i2card.o

else
KDIR := $(MODBASE)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
endif

COMP := $(shell grep "^CONFIG_MODULE_COMPRESS" $(MODBASE)/build/.config)
ifeq ($(findstring PRESS, $(COMP)), )
	COMP = "NONE=y"
endif

install: default
	@echo "config: $(COMP)"
ifneq ($(findstring NONE=y, $(COMP)), )
	@echo modules are uncompressed
	cp -p i2card.ko $(MODBASE)/kernel/drivers/misc/eeprom/
else ifneq ($(findstring GZIP=y, $(COMP)), )
	@echo modules are gzip compressed - actually not supported
	rm -f i2card.ko.gz
else ifneq ($(findstring ZSTD=y, $(COMP)), )
	@echo modules are zstd compressed - actually not supported
	rm -f i2card.ko.zst
else
	@echo modules are xz compressed
	rm -f i2card.ko.xz
	xz -k i2card.ko
	cp -p i2card.ko.xz $(MODBASE)/kernel/drivers/misc/eeprom/
endif

clean:
	@rm -f .*.cmd *.mod *.o *.ko *.ko.*

uninstall:
	rm -f $(MODBASE)/kernel/drivers/misc/eeprom/i2card.k*
