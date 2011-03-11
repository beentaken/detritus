/* $Id: bitcount.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>
#include <stdlib.h>

/*
	The answer to this exercise is that, x &= (x - 1) will always remove
	the last bit in x because:

	x - 1 will decrease the significance of the least significant bit by
	one order.

	Applying and AND operation to x with the result of the above statment
	will ensure that the bits of least significance will be ommited.

	However, I don't know if this will work on a ones complement machine.
*/

unsigned int bitcount(unsigned int x) {
	unsigned int	b;

	for (b = 0; x != 0; x &= (x - 1)) {
		b++;
	}

	return(b);
}

int main(int argc, char *argv[]) {
	unsigned int	x, result;

	if (argc == 2) {
		x = atoi(argv[1]);
		result = bitcount(x);

		printf("x:\t%d\n", x);
		printf("result:\t%d\n", result);
	} else {
		return(1);
	}

	return(0);
}
