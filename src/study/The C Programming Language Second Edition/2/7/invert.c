
#include <stdio.h>
#include <stdlib.h>

int invert(int x, int p, int n) {
	int	nibble;

	nibble = (~(~0 << n) << p) & x;

	return((x - nibble) | ~(nibble + ~(~0 << p) + (~0 << (p + n))));
}

int main(int argc, char *argv[]) {
	int	x, p, n, result;

	if (argc == 4) {
		x = atoi(argv[1]);
		p = atoi(argv[2]);
		n = atoi(argv[3]);
		result = invert(x, p, n);

		printf("x:\t%d\tp:\t%d\tn:\t%d\n", x, p, n);
		printf("result:\t%d\n", result);
	} else {
		return(1);
	}

	return(0);
}
