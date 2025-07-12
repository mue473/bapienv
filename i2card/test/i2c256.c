// C 2025 Rainer Müller
// This program is free software according to GNU General Public License 3 (GPL3).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "../i2card.h"

struct ioctl_wr iowrdat;
unsigned char *data = NULL;
FILE *fp;
int n = 0;

int ioctl_set_dat(unsigned int cardsize, unsigned short	pagesize)
{
	printf("setting EEPROM size %u, pagesize %u.\n", cardsize, pagesize);
	iowrdat.cardsize = cardsize;
	iowrdat.pagesize = pagesize;
	int ret_val = ioctl(fileno(fp), IOCTL_PARMSET, &iowrdat);
	if (ret_val < 0)
		fprintf(stderr, "ioctl_set_dat failed: %s\n", strerror(errno));
	return ret_val;
}

int main(int argc, char **argv)
{
	if ((argc < 3) || ((argv[1][0] != 'r') && (argv[1][0] != 'w'))) {
		fprintf(stderr, "error: r or w to read or write card, and file name required\n");
		return EXIT_FAILURE;
	}

	fp = fopen("/dev/i2card", "r+b");
	if (fp == NULL) {
		fprintf(stderr, "error: fopen card device failed\n");
		return EXIT_FAILURE;
	}

	ioctl_set_dat(256, 8);
	if ((data = calloc(iowrdat.cardsize, 1)) == NULL) {
		fprintf(stderr, "error: can't alloc %u bytes for data\n", iowrdat.cardsize);
		fclose(fp);
		return EXIT_FAILURE;
	}

	if (argv[1][0] == 'r') {		// read from card and write to file
		FILE* pFile = fopen(argv[2], "wb");
		if (pFile) {
			if ((fread((void *)data, 1, iowrdat.cardsize, fp)) != iowrdat.cardsize)
				fprintf(stderr, "error: fread from card failed\n");
			else {
				n = fwrite(data, 1, iowrdat.cardsize, pFile);
				if ((unsigned)n != iowrdat.cardsize)
					fprintf(stderr, "error: writing to File %s failed.\n", argv[2]);
				else printf("%d bytes transferred.\n", n);
			}
			fclose(pFile);
		}
		else
			fprintf(stderr, "error: opening File %s failed.\n", argv[2]);
	}
	else {						// read from file and write to card
		FILE* pFile = fopen(argv[2], "rb");
		if (pFile) {
			n = fread((void *)data, 1, iowrdat.cardsize, pFile);
			if (n < 0)
				fprintf(stderr, "error: reading from File %s failed.\n", argv[2]);
			else {
				if (fwrite((void *)data, 1, n, fp) != (unsigned)n)
					fprintf(stderr, "error: writing to card failed\n");
				else {
					fflush(fp);			// wait until ready
					printf("%d bytes transferred.\n", n);
				}
			}
			fclose(pFile);
		}
		else
			fprintf(stderr, "error: opening File %s failed.\n", argv[2]);
	}

	fclose(fp);
	free(data);
	return EXIT_SUCCESS;
}
