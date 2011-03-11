/* $Id: showargs.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>

int main(int argc, char *argv[]) {
	int	i;

	for (i = 0; i < argc; i++) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}

	return(0);
}
