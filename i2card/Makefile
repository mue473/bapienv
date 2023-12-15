#
# Makefile for the i2card driver for BananaPi
# C 2023 Rainer Müller 
#
.PHONY:	default install clean
MODBASE := /lib/modules/$(shell uname -r)

ifneq ($(KERNELRELEASE),)
obj-m := i2card.o

else
KDIR := $(MODBASE)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
endif

COMP := $(shell grep CONFIG_MODULE_COMPRESS $(MODBASE)/build/.config)

install:
	@echo "config: $(COMP)"
ifeq ($(findstring =y, $(COMP)), )
	@echo modules are uncompressed
	cp -p i2card.ko $(MODBASE)/kernel/drivers/misc/eeprom/
else
	@echo modules are compressed
	rm -f i2card.ko.xz
	xz -k i2card.ko
	cp -p i2card.ko.xz $(MODBASE)/kernel/drivers/misc/eeprom/
endif

clean:
	@rm -f .*.cmd *.mod *.o *.ko *.ko.xz
