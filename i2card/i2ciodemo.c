
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "i2card.h"

#define TEST_DATA_SIZE		128
#define WRITE_DATA_SIZE		(TEST_DATA_SIZE / 2)

struct ioctl_rd iorddat;
struct ioctl_wr iowrdat;
unsigned char *data = NULL;
FILE *fp;

/* Functions for the ioctl calls */

int ioctl_get_dat(void)
{
	int ret_val = ioctl(fileno(fp), IOCTL_PARMGET, &iorddat);
	if (ret_val < 0)
		printf("ioctl_get_dat failed: %s\n", strerror(errno));
	else
		printf("using I2C bus %u, addr 0x%x, EEPROM size %u, pagesize %u.\n",
			iorddat.i2cbus, iorddat.i2caddr, iorddat.cardsize, iorddat.pagesize);
	return ret_val;
}

int ioctl_set_dat(unsigned int cardsize, unsigned short	pagesize)
{
	printf("setting EEPROM size %u, pagesize %u.\n", cardsize, pagesize);
	iowrdat.cardsize = cardsize;
	iowrdat.pagesize = pagesize;
	int ret_val = ioctl(fileno(fp), IOCTL_PARMSET, &iowrdat);
	if (ret_val < 0)
		printf("ioctl_set_dat failed: %s\n", strerror(errno));
	return ret_val;
}

long get_epsize(void)
{
	fseek(fp, 0, SEEK_END);
	long int sz = ftell(fp);
	if (sz < 0L) 
		fprintf(stderr, "%s: error ftell failed, random access not supported.\n", __func__);
	else
		printf("fseek/ftell returns a size of %d\n", sz);	
	return sz;
}

unsigned char *read_data(void)
{
	struct timeval tv = { 0, 0 };
	fseek(fp, 0, SEEK_SET);

	if ((data = malloc(TEST_DATA_SIZE)) == NULL) {
		fprintf(stderr, "%s: can't alloc %u bytes for data\n", __func__, TEST_DATA_SIZE);
		fclose(fp);
		return NULL;
	}
	gettimeofday(&tv, NULL);		// time before
	time_t t_now = tv.tv_sec * 1000000 + tv.tv_usec;
	if ((fread((void *)data, 1, TEST_DATA_SIZE, fp)) != TEST_DATA_SIZE) {
		fprintf(stderr, "%s: error: fread fom card failed\n", __func__);
		fclose(fp);
		free(data);
		return NULL;
	}
	gettimeofday(&tv, NULL);		// time after
	t_now = tv.tv_sec * 1000000 + tv.tv_usec - t_now;
	printf("read took %dµs\n", t_now);
	return data;
}

int write_data(void)
{
	struct timeval tv = { 0, 0 };
	printf("Write data using pagesize %u ", iorddat.pagesize);
	fseek(fp, 20, SEEK_SET);

	gettimeofday(&tv, NULL);		// time before
	time_t t_now = tv.tv_sec * 1000000 + tv.tv_usec;
	if ((fwrite((void *)&data[20], 1, WRITE_DATA_SIZE, fp)) != WRITE_DATA_SIZE) {
		fprintf(stderr, "%s: error writing to card failed\n", __func__);
		return EXIT_FAILURE;
	}
	fflush(fp);						// wait until ready
	gettimeofday(&tv, NULL);		// time after
	t_now = tv.tv_sec * 1000000 + tv.tv_usec - t_now;
	printf("took %dµs\n", t_now);
	return 0;
}

int main(int argc, char **argv)
{	
	fp = fopen("/dev/i2card", "r+b");
	if (fp == NULL) {
		fprintf(stderr, "%s: error fopen device failed\n", __func__);
		return EXIT_FAILURE;
	}

	printf("\n** Start conditions\n");
	ioctl_get_dat();
	if (get_epsize() >= TEST_DATA_SIZE) {
		if (read_data() == NULL) return EXIT_FAILURE;			
		if (data) write_data();		// use default page size	
	}
	
	printf("\n** strange values\n");
	ioctl_set_dat(8000, 20);
	ioctl_get_dat();
	get_epsize();
	
	printf("\n** correct values\n");
	ioctl_set_dat(4096, 8);
	ioctl_get_dat();
	if (get_epsize() >= TEST_DATA_SIZE) {
		if (data) write_data();		// use modified page size	
	}
	
	fclose(fp);
	free(data);
	return EXIT_SUCCESS;
}
