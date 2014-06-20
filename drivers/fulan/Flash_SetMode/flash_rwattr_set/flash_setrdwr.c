#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#include "flash_rwmode_set.h"

int main(int argc, char *argv[])
{
	int fd;

	/*
	 * Parse command line options
	 */
	if(argc != 3)
	{
		fprintf(stderr, "USAGE: %s  <mtd_rwmode_set device> <mtd device>\n", argv[0]);
		exit(1);
	}
	else if(strncmp(argv[2], "/dev/mtd", 8) != 0)
	{
		fprintf(stderr, "'%s' is not a MTD device.  Must specify mtd device: /dev/mtd?\n", argv[2]);
		exit(1);
	}

	fd = open(argv[1], O_RDWR);
	if(fd < 0)
	{
		fprintf(stderr, "Could not open device: %s\n", argv[1]);
		exit(1);
	}

	if(ioctl(fd, MEMSETRDWR, argv[2]))
	{
		fprintf(stderr, "Could not set MTD device read_write: %s\n", argv[2]);
		close(fd);
		exit(1);
	}

	close(fd);

	return 0;
}

