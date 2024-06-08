// i2card.c - character device driver to access i2c card
//				it will create a device at /dev/i2card
//
// C 2023 - 2024 Rainer Müller
// This program is free software according to GNU General Public License 3 (GPL3).

//#define DEBUG

#include <linux/atomic.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include "i2card.h"

#define SUCCESS 0
#define DEVICE_NAME "i2card"
#define ADRSIZE 2
#define DATASIZE 128

static short i2cbusid = 1;
module_param(i2cbusid, short, 0);
MODULE_PARM_DESC(i2cbusid, "id of the bus which connects card reader");

static short i2caddr = 0x50;
module_param(i2caddr, short, 0);
MODULE_PARM_DESC(i2caddr, "address to reach the i2c card in the card reader");

static int eepsize = 8192;
module_param(eepsize, int, 0);
MODULE_PARM_DESC(eepsize, "size of the EEPROM area");

static short pagesize = 32;
module_param(pagesize, short, 0);
MODULE_PARM_DESC(pagesize, "pagesize for writing to the EEPROM area");

enum {CDEV_NOT_USED = 0, CDEV_EXCLUSIVE_OPEN = 1};
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);
static char devbuf[DATASIZE + ADRSIZE];		// first two bytes used for address
static int major;
static loff_t actpos;
static struct class *cls;
static struct i2c_adapter *adapter;
static struct ioctl_rd ioparms;

static int i2card_i2get(int nmbr)
{
    struct i2c_msg msg[2];
    int n, ret;
	pr_devel("%s: read %i bytes from card pos %lli.\n", DEVICE_NAME, nmbr, actpos);
    devbuf[0] = actpos >> 8;
    devbuf[1] = actpos & 0xFF;
    msg[0].addr = i2caddr;
    msg[0].flags = 0;
    msg[0].buf = devbuf;
    msg[0].len = ADRSIZE;
    msg[1].addr = i2caddr;
    msg[1].flags = I2C_M_RD;
    msg[1].buf = devbuf + ADRSIZE;
    msg[1].len = nmbr;
	for(n = 0; n < 15; n++) {			// acknowledge polling
    	ret = i2c_transfer(adapter, msg, 2);
		if (ret > 0) return ret;		// should be 2 if OK
		usleep_range(1000, 1500);
	}
	pr_alert("%s: i2c_transfer for read operation failed  with code %d.\n",
					DEVICE_NAME, ret);
	return ret;
}

static int i2card_i2put(int nmbr)
{
    struct i2c_msg msg;
    int n, ret;
	pr_devel("%s: write %i bytes to card pos %lli.\n", DEVICE_NAME, nmbr, actpos);
    devbuf[0] = actpos >> 8;
    devbuf[1] = actpos & 0xFF;
    msg.addr = i2caddr;
    msg.flags = 0;
    msg.buf = devbuf;
    msg.len = nmbr + ADRSIZE;
	for(n = 0; n < 15; n++) {			// acknowledge polling
    	ret = i2c_transfer(adapter, &msg, 1);
		if (ret > 0) return ret;		// should be 1 if OK
		usleep_range(1000, 1500);
	}
	pr_alert("%s: i2c_transfer for write operation failed with code %d.\n",
					DEVICE_NAME, ret);
	return ret;
}

static void i2card_checksizes(void)
{
	if (!is_power_of_2(ioparms.cardsize))
		pr_alert("%s: EEPROM size looks suspicious (no power of 2)!\n", DEVICE_NAME);
	if (!is_power_of_2(ioparms.pagesize))
		pr_alert("%s: page size looks suspicious (no power of 2)!\n", DEVICE_NAME);
}

static int i2card_close(struct inode *devnode, struct file *filp)
{
	pr_info("%s: close was called.\n", DEVICE_NAME);
	atomic_set(&already_open, CDEV_NOT_USED);
	module_put(THIS_MODULE);
	return SUCCESS;
}

static int i2card_open(struct inode *devnode, struct file *filp)
{
	int ret;
	pr_info("%s: opened using I2C bus %d, addr 0x%x, EEPROM size %d, pagesize %d.\n",
			DEVICE_NAME, i2cbusid, i2caddr, eepsize, pagesize);
	if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
		return -EBUSY;
	try_module_get(THIS_MODULE);
	actpos = 0;
	ret = i2card_i2get(1);			// check if card is accessible
	if (ret != 2) {
		pr_devel("%s: reading one byte failed with code %d.\n", DEVICE_NAME, ret);
		i2card_close(devnode, filp);
		return ret;
	}
	ioparms.cardsize = eepsize;
	ioparms.pagesize = pagesize;
	return SUCCESS;
}

static loff_t i2card_seek(struct file *filp, loff_t offset, int whence)
{
	pr_devel("%s: seek mode %i to offset %lli.\n", DEVICE_NAME, whence, offset);
	switch (whence) {
		case SEEK_SET:	break;
		case SEEK_CUR:	offset += actpos;	break;
		case SEEK_END:  offset += ioparms.cardsize;	break;
		default:		return -EINVAL;
	}
	if ((offset < 0) || (offset > ioparms.cardsize))
		return -ERANGE;
	actpos = offset;
	return actpos;
}

static long i2card_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	long l;
	switch (cmd) {
		case IOCTL_PARMGET:	l = copy_to_user((void *) arg, &ioparms, sizeof ioparms);
							break;
		case IOCTL_PARMSET: l = copy_from_user(&ioparms.cardsize, (void *) arg,
													sizeof(struct ioctl_wr));
							pr_info("%s: parms modified: EEPROM size %d, pagesize %d.\n",
									DEVICE_NAME, ioparms.cardsize, ioparms.pagesize);
							i2card_checksizes();
							break;
		default:	pr_alert("%s: ioctl operation %u is not supported.\n", DEVICE_NAME, cmd);
					return -ENOTSUPP;
	}
	return (l ? -EIO : SUCCESS);		
}

static ssize_t i2card_read(struct file *filp, char __user *buff, size_t count, loff_t *offset)
{
	int toread, res;
	pr_devel("%s: read into buffer with size %u from offset %lli.\n", DEVICE_NAME, count, *offset);
	toread = ioparms.cardsize - actpos;
	if (toread > DATASIZE)
		toread = DATASIZE;
	if (toread > count)
		toread = count;
	if (toread < 1)
		return 0;						// end of file
	res = i2card_i2get(toread);
	if (res < 0)						// card access problems
		return res;
	count = toread - copy_to_user(buff, devbuf + ADRSIZE, toread);
	actpos += count;
	*offset = actpos;
	return count;
}

static ssize_t i2card_write(struct file *filp, const char __user *buff, size_t len, loff_t *offset)
{ 
	int towrite, res;
	pr_devel("%s: write from buffer with size %u to offset %lli.\n", DEVICE_NAME, len, *offset);
	if (ioparms.pagesize > DATASIZE)
		ioparms.pagesize = DATASIZE;
	towrite = ioparms.pagesize - (actpos & (ioparms.pagesize - 1));		// fit in page
	if (towrite > len)
		towrite = len;
	if ((actpos + towrite) > ioparms.cardsize)
		return -ENOSPC;
	len = towrite - copy_from_user(devbuf + ADRSIZE, buff, towrite);
    res = i2card_i2put(len);
	if (res < 0)						// card access problems
		return res;
	actpos += len;
	*offset = actpos;
	return len;
}

static struct file_operations fops = {
	.owner   = THIS_MODULE,
	.llseek  = i2card_seek,
	.read    = i2card_read,
	.write   = i2card_write,
	.unlocked_ioctl = i2card_ioctl,
	.open    = i2card_open,
	.release = i2card_close,
};

static int __init i2card_init(void)
{
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if (major < 0) {
		pr_alert("%s: registering device failed with %d.\n", DEVICE_NAME, major);
		return major;
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
	cls = class_create(DEVICE_NAME);
#else
	cls = class_create(THIS_MODULE, DEVICE_NAME);
#endif
	device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	pr_info("%s: created with major number %d using I2C bus %d, EEPROM size %d.\n",
			DEVICE_NAME, major, i2cbusid, eepsize);
	ioparms.i2cbus = i2cbusid;
	ioparms.i2caddr = i2caddr;
	ioparms.cardsize = eepsize;
	ioparms.pagesize = pagesize;
	i2card_checksizes();
	adapter = i2c_get_adapter(i2cbusid);
    if (adapter == NULL)
		pr_alert("%s: using i2c_adapter for bus %d failed.\n", DEVICE_NAME, i2cbusid);
	// TODO: why SUCCESS when failed?
	return SUCCESS;
}

static void __exit i2card_exit(void)
{
	pr_info("%s: unloading because exit was called.\n", DEVICE_NAME);
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major, DEVICE_NAME);
	i2c_put_adapter(adapter);
}

module_init(i2card_init);
module_exit(i2card_exit);

MODULE_DESCRIPTION("driver for removable i2c card");
MODULE_AUTHOR("Rainer Müller - mue473");
MODULE_LICENSE("GPL");
