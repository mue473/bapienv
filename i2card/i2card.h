// i2card.h - ioctl interface for character device driver to access i2c card
//
// C 2024 Rainer Müller
// This program is free software according to GNU General Public License 3 (GPL3).

#ifndef I2CARD_H
#define I2CARD_H

#include <linux/ioctl.h>

struct ioctl_rd {
	unsigned short	i2cbus;
	unsigned short	i2caddr;
	unsigned int	cardsize;
	unsigned short	pagesize;
};

struct ioctl_wr {
	unsigned int	cardsize;
	unsigned short	pagesize;
};

#define IOC_MAGIC '\x69'

#define IOCTL_PARMGET _IOR(IOC_MAGIC, 1, struct ioctl_rd)
#define IOCTL_PARMSET _IOW(IOC_MAGIC, 2, struct ioctl_wr)

#endif		// I2CARD_H
