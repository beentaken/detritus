/* $Id: nethack-stats.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include "logdata.h"

int main(int argc, char **argv) {
	Data	data;

	getData(&data);
	printData(&data);

	return(0);
}
