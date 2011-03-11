/* $Id: main.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <unistd.h>
#include <stdlib.h>

#include "console.h"

int main(int argc, char **argv) {
	unsigned int	port = 31337;

	int		rtcode = 0;

	/* The port to listen on can be set on the command line */
	if (argc > 1) {
		port = atoi(argv[1]);
	}

	/* Fire up the console using stdin and stdout for I/O */
	rtcode = console_io(0, 1);

	return(rtcode);
}
