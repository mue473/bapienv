/* ======================================================================= */
/*   bpipek.c - application to handle events from BPi's Power Event Key    */
/*   (c) 2023 Rainer Müller                                                */
/* ======================================================================= */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/input.h>

#define LONGTIME 1250000

static char evinput[] = "/dev/input/event0";
static char conffile[] = "/etc/bpipekconf";

void keyaction(char act)
{
	char line[255];
	
	FILE *cfile = fopen(conffile, "r");
	if (cfile == NULL) {
		fprintf(stderr, "error opening configuration file %s: %s\n",
						conffile, strerror(errno));
		return;
	}
	while (fgets(line, sizeof(line), cfile) != NULL) {
		if (line[0] == act) {
			printf("%s\n", line);
			int rc = system(line + 2);
			if (rc)
				fprintf(stderr, "Error occured: system return code is %d\n", rc);
		}
	}
	fclose(cfile);
}

int main(/*int argc, char **argv*/)
{
	struct input_event ev;
	int64_t ptime = 0;
	int fd;

	if ((fd = open(evinput, O_RDONLY)) < 0) {
		fprintf(stderr, "error opening power event queue: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	while (1) {
		if (read(fd, &ev, sizeof(struct input_event)) < (int)sizeof(struct input_event)) {
			fprintf(stderr, "error reading power event queue: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		printf("Event: time %ld.%06ld,  type 0x%02x, code 0x%02x, value 0x%04x\n",
					ev.time.tv_sec, ev.time.tv_usec, ev.type, ev.code, ev.value);
		if (ev.type == EV_KEY && ev.code == KEY_POWER) {
			int64_t timestamp = (int64_t) ev.time.tv_sec * 1000000 + ev.time.tv_usec;
			if (ev.value) {
				keyaction('P');			// PEK presesd
				ptime = timestamp;
			}
			else keyaction((timestamp - ptime) < LONGTIME ? 'S' : 'L');	// PEK released
		}
	}
	return 0;
}

