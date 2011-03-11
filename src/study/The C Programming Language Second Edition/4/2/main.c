/* $Id: main.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>
#include "atof.h"

int main(int argc, char *argv[]) {

	if (argc == 2) {
		printf("Input:\t%s\nOutput:\t%f\n", argv[1], atof(argv[1]));
	} else {
		return(1);
	}

	return(0);
}
