#
# Makefile for the sun4i_can driver for BananaPi
# C 2019 - 2022 Rainer Müller 
#
.PHONY:	default install clean
MODBASE := /lib/modules/$(shell uname -r)

ifneq ($(KERNELRELEASE),)
obj-m := sun4i_can.o

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
	cp -p sun4i_can.ko $(MODBASE)/kernel/drivers/net/can/
else
	@echo modules are compressed
	rm -f sun4i_can.ko.xz
	xz -k sun4i_can.ko
	cp -p sun4i_can.ko.xz $(MODBASE)/kernel/drivers/net/can/
endif

clean:
	@rm -f .*.cmd *.mod *.o *.ko *.ko.xz

